#include <fstream>
#include <iostream>

#include <OglUI/UI_Image.h>
#include <OglUI/UI_Group.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_DDSource.h>
#include <OglUI/UI_DDTarget.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_TextClickable.h>
#include <OglUI/UI_ContextualMenu.h>

#include <OglWindow/Window_MessageBox.h>
#include <OglWindow/Window_SaveFileDialog.h>
#include <OglWindow/Window_OpenFileDialog.h>

#include <OglTools/Tools_SystemCalls.h>
#include <OglTools/Tools_PathParser.h>
#include <OglTools/Tools_String.h>

#include <OglCore/Core_ModelManager.h>
#include <OglCore/Core_ShaderManager.h>
#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_TextureManager.h>

#include "Editor_AssetView.h"
#include "Editor_AssetBrowser.h"
#include "Editor_EditorActions.h"
#include "Editor_MaterialEditor.h"
#include "Editor_AssetProperties.h"
#include "Editor_EditorResources.h"

#define FILENAMES_CHARS Editor::Editor_AssetBrowser::__FILENAMES_CHARS

const std::string FILENAMES_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-_=+ 0123456789()[]";

std::string GetAssociatedMetaFile(const std::string& p_assetPath)
{
	return p_assetPath + ".meta";
}

void RenameAsset(const std::string& p_prev, const std::string& p_new)
{
	std::filesystem::rename(p_prev, p_new);

	if (const std::string previousMetaPath = GetAssociatedMetaFile(p_prev); std::filesystem::exists(previousMetaPath))
	{
		if (const std::string newMetaPath = GetAssociatedMetaFile(p_new); !std::filesystem::exists(newMetaPath))
		{
			std::filesystem::rename(previousMetaPath, newMetaPath);
		}
		else
		{
			//OVLOG_ERROR(newMetaPath + " is already existing, .meta creation failed");
		}
	}
}

void RemoveAsset(const std::string& p_toDelete)
{
	std::filesystem::remove(p_toDelete);

	if (const std::string metaPath = GetAssociatedMetaFile(p_toDelete); std::filesystem::exists(metaPath))
	{
		std::filesystem::remove(metaPath);
	}
}

class Editor_TexturePreview : public UI::UI_IPlugin
{
public:
	Editor_TexturePreview() : image(0, { 80, 80 })
	{

	}

	void SetPath(const std::string& p_path)
	{
		texture = Core::Core_ServiceLocator::Get<Core::Core_TextureManager>()[p_path];
	}

	virtual void Execute() override
	{
		if (ImGui::IsItemHovered())
		{
			if (texture)
			{
				image.mTextureID.mId = texture->id;
			}

			ImGui::BeginTooltip();
			image.Draw();
			ImGui::EndTooltip();
		}
	}

	Render::Render_Texture* texture = nullptr;
	UI::UI_Image image;
};

class Editor_BrowserItemContextualMenu : public UI::UI_ContextualMenu
{
public:
	Editor_BrowserItemContextualMenu(const std::string p_filePath, bool p_protected = false) : m_protected(p_protected), filePath(p_filePath) {}

	virtual void CreateList()
	{
		if (!m_protected)
		{
			auto& renameMenu = CreateWidget<UI::UI_MenuList>("Rename to...");
			auto& deleteAction = CreateWidget<UI::UI_MenuItem>("Delete");

			auto& nameEditor = renameMenu.CreateWidget<UI::UI_InputText>("");
			nameEditor.mSelectAllOnClick = true;

			renameMenu.mClickedEvent += [this, &nameEditor]
				{
					nameEditor.mContent = Tools::Tools_PathParser::GetElementName(filePath);

					if (!std::filesystem::is_directory(filePath))
					{
						if (size_t pos = nameEditor.mContent.rfind('.'); pos != std::string::npos)
						{
							nameEditor.mContent = nameEditor.mContent.substr(0, pos);
						}
					}
				};

			deleteAction.mClickedEvent += [this] { DeleteItem(); };

			nameEditor.mEnterPressedEvent += [this](std::string p_newName)
				{
					if (!std::filesystem::is_directory(filePath))
					{
						p_newName += '.' + Tools::Tools_PathParser::GetExtension(filePath);
					}

					p_newName.erase(std::remove_if(p_newName.begin(), p_newName.end(), [](auto& c)
						{
							return std::find(FILENAMES_CHARS.begin(), FILENAMES_CHARS.end(), c) == FILENAMES_CHARS.end();
						}), p_newName.end());

					std::string containingFolderPath = Tools::Tools_PathParser::GetContainingFolder(filePath);
					std::string newPath = containingFolderPath + p_newName;
					std::string oldPath = filePath;

					if (filePath != newPath && !std::filesystem::exists(newPath))
					{
						filePath = newPath;
					}

					if (std::filesystem::is_directory(oldPath))
					{
						filePath += '\\';
					}

					RenamedEvent.Invoke(oldPath, newPath);
				};
		}
	}

	virtual void Execute() override
	{
		if (mWidgets.size() > 0)
		{
			UI::UI_ContextualMenu::Execute();
		}
	}

	virtual void DeleteItem() = 0;

public:
	bool m_protected;
	std::string filePath;
	Tools::Tools_Event<std::string> DestroyedEvent;
	Tools::Tools_Event<std::string, std::string> RenamedEvent;
};

class Editor_FolderContextualMenu : public Editor_BrowserItemContextualMenu
{
public:
	Editor_FolderContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_BrowserItemContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& showInExplorer = CreateWidget<UI::UI_MenuItem>("Show in explorer");
		showInExplorer.mClickedEvent += [this]
			{
				Tools::Tools_SystemCalls::ShowInExplorer(filePath);
			};

		if (!m_protected)
		{
			auto& importAssetHere = CreateWidget<UI::UI_MenuItem>("Import Here...");
			importAssetHere.mClickedEvent += [this]
				{
					if (EDITOR_EXEC(ImportAssetAtLocation(filePath)))
					{
						UI::UI_TreeNode* pluginOwner = reinterpret_cast<UI::UI_TreeNode*>(userData);
						pluginOwner->Close();
						EDITOR_EXEC(DelayAction(std::bind(&UI::UI_TreeNode::Open, pluginOwner)));
					}
				};

			auto& createMenu = CreateWidget<UI::UI_MenuList>("Create..");

			auto& createFolderMenu = createMenu.CreateWidget<UI::UI_MenuList>("Folder");
			auto& createSceneMenu = createMenu.CreateWidget<UI::UI_MenuList>("Scene");
			auto& createShaderMenu = createMenu.CreateWidget<UI::UI_MenuList>("Shader");
			auto& createMaterialMenu = createMenu.CreateWidget<UI::UI_MenuList>("Material");

			auto& createStandardShaderMenu = createShaderMenu.CreateWidget<UI::UI_MenuList>("Standard template");
			auto& createStandardPBRShaderMenu = createShaderMenu.CreateWidget<UI::UI_MenuList>("Standard PBR template");
			auto& createUnlitShaderMenu = createShaderMenu.CreateWidget<UI::UI_MenuList>("Unlit template");
			auto& createLambertShaderMenu = createShaderMenu.CreateWidget<UI::UI_MenuList>("Lambert template");

			auto& createEmptyMaterialMenu = createMaterialMenu.CreateWidget<UI::UI_MenuList>("Empty");
			auto& createStandardMaterialMenu = createMaterialMenu.CreateWidget<UI::UI_MenuList>("Standard");
			auto& createStandardPBRMaterialMenu = createMaterialMenu.CreateWidget<UI::UI_MenuList>("Standard PBR");
			auto& createUnlitMaterialMenu = createMaterialMenu.CreateWidget<UI::UI_MenuList>("Unlit");
			auto& createLambertMaterialMenu = createMaterialMenu.CreateWidget<UI::UI_MenuList>("Lambert");

			auto& createFolder = createFolderMenu.CreateWidget<UI::UI_InputText>("");
			auto& createScene = createSceneMenu.CreateWidget<UI::UI_InputText>("");

			auto& createEmptyMaterial = createEmptyMaterialMenu.CreateWidget<UI::UI_InputText>("");
			auto& createStandardMaterial = createStandardMaterialMenu.CreateWidget<UI::UI_InputText>("");
			auto& createStandardPBRMaterial = createStandardPBRMaterialMenu.CreateWidget<UI::UI_InputText>("");
			auto& createUnlitMaterial = createUnlitMaterialMenu.CreateWidget<UI::UI_InputText>("");
			auto& createLambertMaterial = createLambertMaterialMenu.CreateWidget<UI::UI_InputText>("");

			auto& createStandardShader = createStandardShaderMenu.CreateWidget<UI::UI_InputText>("");
			auto& createStandardPBRShader = createStandardPBRShaderMenu.CreateWidget<UI::UI_InputText>("");
			auto& createUnlitShader = createUnlitShaderMenu.CreateWidget<UI::UI_InputText>("");
			auto& createLambertShader = createLambertShaderMenu.CreateWidget<UI::UI_InputText>("");

			createFolderMenu.mClickedEvent += [&createFolder] { createFolder.mContent = ""; };
			createSceneMenu.mClickedEvent += [&createScene] { createScene.mContent = ""; };
			createStandardShaderMenu.mClickedEvent += [&createStandardShader] { createStandardShader.mContent = ""; };
			createStandardPBRShaderMenu.mClickedEvent += [&createStandardPBRShader] { createStandardPBRShader.mContent = ""; };
			createUnlitShaderMenu.mClickedEvent += [&createUnlitShader] { createUnlitShader.mContent = ""; };
			createLambertShaderMenu.mClickedEvent += [&createLambertShader] { createLambertShader.mContent = ""; };
			createEmptyMaterialMenu.mClickedEvent += [&createEmptyMaterial] { createEmptyMaterial.mContent = ""; };
			createStandardMaterialMenu.mClickedEvent += [&createStandardMaterial] { createStandardMaterial.mContent = ""; };
			createStandardPBRMaterialMenu.mClickedEvent += [&createStandardPBRMaterial] { createStandardPBRMaterial.mContent = ""; };
			createUnlitMaterialMenu.mClickedEvent += [&createUnlitMaterial] { createUnlitMaterial.mContent = ""; };
			createLambertMaterialMenu.mClickedEvent += [&createLambertMaterial] { createLambertMaterial.mContent = ""; };

			createFolder.mEnterPressedEvent += [this](std::string newFolderName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + (!fails ? newFolderName : newFolderName + " (" + std::to_string(fails) + ')');

						++fails;
					} while (std::filesystem::exists(finalPath));

					std::filesystem::create_directory(finalPath);

					ItemAddedEvent.Invoke(finalPath);
					Close();
				};

			createScene.mEnterPressedEvent += [this](std::string newSceneName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + (!fails ? newSceneName : newSceneName + " (" + std::to_string(fails) + ')') + ".ovscene";

						++fails;
					} while (std::filesystem::exists(finalPath));

					std::ofstream outfile(finalPath);
					outfile << "<root><scene><actors><actor><name>Directional Light</name><tag></tag><active>true</active><id>1</id><parent>0</parent><components><component><type>class OvCore::ECS::Components::CDirectionalLight</type><data><diffuse><x>1</x><y>1</y><z>1</z></diffuse><specular><x>1</x><y>1</y><z>1</z></specular><intensity>0.75</intensity></data></component><component><type>class OvCore::ECS::Components::CTransform</type><data><position><x>0</x><y>10</y><z>0</z></position><rotation><x>0.81379771</x><y>-0.17101006</y><z>0.29619816</z><w>0.46984628</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor><actor><name>Ambient Light</name><tag></tag><active>true</active><id>2</id><parent>0</parent><components><component><type>class OvCore::ECS::Components::CAmbientSphereLight</type><data><ambient><x>1</x><y>1</y><z>1</z></ambient><intensity>0.1</intensity><radius>10000</radius></data></component><component><type>class OvCore::ECS::Components::CTransform</type><data><position><x>0</x><y>0</y><z>0</z></position><rotation><x>0</x><y>0</y><z>0</z><w>1</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor><actor><name>Main Camera</name><tag></tag><active>true</active><id>3</id><parent>0</parent><components><component><type>class OvCore::ECS::Components::CCamera</type><data><fov>45</fov><near>0.1</near><far>1000</far><clear_color><x>0.1921569</x><y>0.3019608</y><z>0.47450981</z></clear_color></data></component><component><type>class OvCore::ECS::Components::CTransform</type><data><position><x>0</x><y>3</y><z>8</z></position><rotation><x>-7.5904039e-09</x><y>0.98480773</y><z>-0.17364819</z><w>-4.3047311e-08</w></rotation><scale><x>1</x><y>1</y><z>1</z></scale></data></component></components><behaviours/></actor></actors></scene></root>" << std::endl; // Empty scene content

					ItemAddedEvent.Invoke(finalPath);
					Close();
				};

			createStandardShader.mEnterPressedEvent += [this](std::string newShaderName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

						++fails;
					} while (std::filesystem::exists(finalPath));

					std::filesystem::copy_file(EDITOR_CONTEXT(mEngineAssetsPath) + "Shaders\\Standard.glsl", finalPath);
					ItemAddedEvent.Invoke(finalPath);
					Close();
				};

			createStandardPBRShader.mEnterPressedEvent += [this](std::string newShaderName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

						++fails;
					} while (std::filesystem::exists(finalPath));

					std::filesystem::copy_file(EDITOR_CONTEXT(mEngineAssetsPath) + "Shaders\\StandardPBR.glsl", finalPath);
					ItemAddedEvent.Invoke(finalPath);
					Close();
				};

			createUnlitShader.mEnterPressedEvent += [this](std::string newShaderName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

						++fails;
					} while (std::filesystem::exists(finalPath));

					std::filesystem::copy_file(EDITOR_CONTEXT(mEngineAssetsPath) + "Shaders\\Unlit.glsl", finalPath);
					ItemAddedEvent.Invoke(finalPath);
					Close();
				};

			createLambertShader.mEnterPressedEvent += [this](std::string newShaderName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + '\\' + (!fails ? newShaderName : newShaderName + " (" + std::to_string(fails) + ')') + ".glsl";

						++fails;
					} while (std::filesystem::exists(finalPath));

					std::filesystem::copy_file(EDITOR_CONTEXT(mEngineAssetsPath) + "Shaders\\Lambert.glsl", finalPath);
					ItemAddedEvent.Invoke(finalPath);
					Close();
				};

			createEmptyMaterial.mEnterPressedEvent += [this](std::string materialName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

						++fails;
					} while (std::filesystem::exists(finalPath));

					{
						std::ofstream outfile(finalPath);
						outfile << "<root><shader>?</shader></root>" << std::endl; // Empty material content
					}

					ItemAddedEvent.Invoke(finalPath);

					if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
					{
						auto& materialEditor = EDITOR_PANEL(Editor::Editor_MaterialEditor, "Material Editor");
						materialEditor.SetTarget(*instance);
						materialEditor.Open();
						materialEditor.Focus();
						materialEditor.Preview();
					}
					Close();
				};

			createStandardMaterial.mEnterPressedEvent += [this](std::string materialName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

						++fails;
					} while (std::filesystem::exists(finalPath));

					{
						std::ofstream outfile(finalPath);
						outfile << "<root><shader>:Shaders\\Standard.glsl</shader></root>" << std::endl;
					}

					ItemAddedEvent.Invoke(finalPath);

					if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
					{
						auto& materialEditor = EDITOR_PANEL(Editor::Editor_MaterialEditor, "Material Editor");
						materialEditor.SetTarget(*instance);
						materialEditor.Open();
						materialEditor.Focus();
						materialEditor.Preview();
					}
					Close();
				};

			createStandardPBRMaterial.mEnterPressedEvent += [this](std::string materialName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

						++fails;
					} while (std::filesystem::exists(finalPath));

					{
						std::ofstream outfile(finalPath);
						outfile << "<root><shader>:Shaders\\StandardPBR.glsl</shader></root>" << std::endl; // Empty standard material content
					}

					ItemAddedEvent.Invoke(finalPath);

					if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
					{
						auto& materialEditor = EDITOR_PANEL(Editor::Editor_MaterialEditor, "Material Editor");
						materialEditor.SetTarget(*instance);
						materialEditor.Open();
						materialEditor.Focus();
						materialEditor.Preview();
					}
					Close();
				};

			createUnlitMaterial.mEnterPressedEvent += [this](std::string materialName)
				{
					std::string newSceneName = "Material";
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

						++fails;
					} while (std::filesystem::exists(finalPath));

					{
						std::ofstream outfile(finalPath);
						outfile << "<root><shader>:Shaders\\Unlit.glsl</shader></root>" << std::endl; // Empty unlit material content
					}

					ItemAddedEvent.Invoke(finalPath);

					if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
					{
						auto& materialEditor = EDITOR_PANEL(Editor::Editor_MaterialEditor, "Material Editor");
						materialEditor.SetTarget(*instance);
						materialEditor.Open();
						materialEditor.Focus();
						materialEditor.Preview();
					}
					Close();
				};

			createLambertMaterial.mEnterPressedEvent += [this](std::string materialName)
				{
					size_t fails = 0;
					std::string finalPath;

					do
					{
						finalPath = filePath + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

						++fails;
					} while (std::filesystem::exists(finalPath));

					{
						std::ofstream outfile(finalPath);
						outfile << "<root><shader>:Shaders\\Lambert.glsl</shader></root>" << std::endl;
					}

					ItemAddedEvent.Invoke(finalPath);

					if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
					{
						auto& materialEditor = EDITOR_PANEL(Editor::Editor_MaterialEditor, "Material Editor");
						materialEditor.SetTarget(*instance);
						materialEditor.Open();
						materialEditor.Focus();
						materialEditor.Preview();
					}
					Close();
				};

			Editor_BrowserItemContextualMenu::CreateList();
		}
	}

	virtual void DeleteItem() override
	{
		Window::Window_MessageBox message("Delete folder", "Deleting a folder (and all its content) is irreversible, are you sure that you want to delete \"" + filePath + "\"?", Window::Window_MessageBox::EMessageType::WARNING, Window::Window_MessageBox::EButtonLayout::YES_NO);

		if (message.GetUserAction() == Window::Window_MessageBox::EUserAction::YES)
		{
			if (std::filesystem::exists(filePath) == true)
			{
				EDITOR_EXEC(PropagateFolderDestruction(filePath));
				std::filesystem::remove_all(filePath);
				DestroyedEvent.Invoke(filePath);
			}
		}
	}

public:
	Tools::Tools_Event<std::string> ItemAddedEvent;
};

class Editor_ScriptFolderContextualMenu : public Editor_FolderContextualMenu
{
public:
	Editor_ScriptFolderContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_FolderContextualMenu(p_filePath, p_protected) {}

	void CreateScript(const std::string& p_name, const std::string& p_path)
	{
		std::string fileContent = "local " + p_name + " =\n{\n}\n\nfunction " + p_name + ":OnStart()\nend\n\nfunction " + p_name + ":OnUpdate(deltaTime)\nend\n\nreturn " + p_name;

		std::ofstream outfile(p_path);
		outfile << fileContent << std::endl;

		ItemAddedEvent.Invoke(p_path);
		Close();
	}

	virtual void CreateList() override
	{
		Editor_FolderContextualMenu::CreateList();

		auto& newScriptMenu = CreateWidget<UI::UI_MenuList>("New script...");
		auto& nameEditor = newScriptMenu.CreateWidget<UI::UI_InputText>("");

		newScriptMenu.mClickedEvent += [this, &nameEditor]
			{
				nameEditor.mContent = Tools::Tools_PathParser::GetElementName("");
			};

		nameEditor.mEnterPressedEvent += [this](std::string p_newName)
			{
				p_newName.erase(std::remove_if(p_newName.begin(), p_newName.end(), [](auto& c)
					{
						return std::find(FILENAMES_CHARS.begin(), FILENAMES_CHARS.end(), c) == FILENAMES_CHARS.end();
					}), p_newName.end());

				std::string newPath = filePath + p_newName + ".lua";

				if (!std::filesystem::exists(newPath))
				{
					CreateScript(p_newName, newPath);
				}
			};
	}
};

class Editor_FileContextualMenu : public Editor_BrowserItemContextualMenu
{
public:
	Editor_FileContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_BrowserItemContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<UI::UI_MenuItem>("Open");

		editAction.mClickedEvent += [this]
			{
				Tools::Tools_SystemCalls::OpenFile(filePath);
			};

		if (!m_protected)
		{
			auto& duplicateAction = CreateWidget<UI::UI_MenuItem>("Duplicate");

			duplicateAction.mClickedEvent += [this]
				{
					std::string filePathWithoutExtension = filePath;

					if (size_t pos = filePathWithoutExtension.rfind('.'); pos != std::string::npos)
					{
						filePathWithoutExtension = filePathWithoutExtension.substr(0, pos);
					}

					std::string extension = "." + Tools::Tools_PathParser::GetExtension(filePath);

					auto filenameAvailable = [&extension](const std::string& target)
						{
							return !std::filesystem::exists(target + extension);
						};

					const auto newNameWithoutExtension = Tools::Tools_String::GenerateUnique(filePathWithoutExtension, filenameAvailable);

					std::string finalPath = newNameWithoutExtension + extension;
					std::filesystem::copy(filePath, finalPath);

					DuplicateEvent.Invoke(finalPath);
				};
		}

		Editor_BrowserItemContextualMenu::CreateList();


		auto& editMetadata = CreateWidget<UI::UI_MenuItem>("Properties");

		editMetadata.mClickedEvent += [this]
			{
				auto& panel = EDITOR_PANEL(Editor::Editor_AssetProperties, "Asset Properties");
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				panel.SetTarget(resourcePath);
				panel.Open();
				panel.Focus();
			};
	}

	virtual void DeleteItem() override
	{
		Window::Window_MessageBox message("Delete file", "Deleting a file is irreversible, are you sure that you want to delete \"" + filePath + "\"?", Window::Window_MessageBox::EMessageType::WARNING, Window::Window_MessageBox::EButtonLayout::YES_NO);

		if (message.GetUserAction() == Window::Window_MessageBox::EUserAction::YES)
		{
			RemoveAsset(filePath);
			DestroyedEvent.Invoke(filePath);
			EDITOR_EXEC(PropagateFileRename(filePath, "?"));
		}
	}

public:
	Tools::Tools_Event<std::string> DuplicateEvent;
};

template<typename Resource, typename ResourceLoader>
class Editor_PreviewableContextualMenu : public Editor_FileContextualMenu
{
public:
	Editor_PreviewableContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& previewAction = CreateWidget<UI::UI_MenuItem>("Preview");

		previewAction.mClickedEvent += [this]
			{
				Resource* resource = Core::Core_ServiceLocator::Get<ResourceLoader>()[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
				auto& assetView = EDITOR_PANEL(Editor::Editor_AssetView, "Asset View");
				assetView.SetResource(resource);
				assetView.Open();
				assetView.Focus();
			};

		Editor_FileContextualMenu::CreateList();
	}
};

class Editor_ShaderContextualMenu : public Editor_FileContextualMenu
{
public:
	Editor_ShaderContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		Editor_FileContextualMenu::CreateList();

		auto& compileAction = CreateWidget<UI::UI_MenuItem>("Compile");

		compileAction.mClickedEvent += [this]
			{
				auto& shaderManager = OVSERVICE(Core::Core_ShaderManager);
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (shaderManager.IsResourceRegistered(resourcePath))
				{
					Render::Render_ShaderLoader::Recompile(*shaderManager[resourcePath], filePath);
				}
				else
				{
					Render::Render_Shader* shader = OVSERVICE(Core::Core_ShaderManager)[resourcePath];
					if (shader)
					{
						//OVLOG_INFO("[COMPILE] \"" + filePath + "\": Success!");
					}
				}

			};
	}
};

class Editor_ModelContextualMenu : public Editor_PreviewableContextualMenu<Render::Render_Model, Core::Core_ModelManager>
{
public:
	Editor_ModelContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& reloadAction = CreateWidget<UI::UI_MenuItem>("Reload");

		reloadAction.mClickedEvent += [this]
			{
				auto& modelManager = OVSERVICE(Core::Core_ModelManager);
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (modelManager.IsResourceRegistered(resourcePath))
				{
					modelManager.Core_AResourceManager::ReloadResource(resourcePath);
				}
			};

		if (!m_protected)
		{
			auto& generateMaterialsMenu = CreateWidget<UI::UI_MenuList>("Generate materials...");

			generateMaterialsMenu.CreateWidget<UI::UI_MenuItem>("Standard").mClickedEvent += [this]
				{
					auto& modelManager = OVSERVICE(Core::Core_ModelManager);
					std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
					if (auto model = modelManager.GetResource(resourcePath))
					{
						for (const std::string& materialName : model->GetMaterialNames())
						{
							size_t fails = 0;
							std::string finalPath;

							do
							{
								finalPath = Tools::Tools_PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

								++fails;
							} while (std::filesystem::exists(finalPath));

							{
								std::ofstream outfile(finalPath);
								outfile << "<root><shader>:Shaders\\Standard.glsl</shader></root>" << std::endl; // Empty standard material content
							}

							DuplicateEvent.Invoke(finalPath);
						}
					}
				};

			generateMaterialsMenu.CreateWidget<UI::UI_MenuItem>("StandardPBR").mClickedEvent += [this]
				{
					auto& modelManager = OVSERVICE(Core::Core_ModelManager);
					std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
					if (auto model = modelManager.GetResource(resourcePath))
					{
						for (const std::string& materialName : model->GetMaterialNames())
						{
							size_t fails = 0;
							std::string finalPath;

							do
							{
								finalPath = Tools::Tools_PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

								++fails;
							} while (std::filesystem::exists(finalPath));

							{
								std::ofstream outfile(finalPath);
								outfile << "<root><shader>:Shaders\\StandardPBR.glsl</shader></root>" << std::endl;
							}

							DuplicateEvent.Invoke(finalPath);
						}
					}
				};

			generateMaterialsMenu.CreateWidget<UI::UI_MenuItem>("Unlit").mClickedEvent += [this]
				{
					auto& modelManager = OVSERVICE(Core::Core_ModelManager);
					std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
					if (auto model = modelManager.GetResource(resourcePath))
					{
						for (const std::string& materialName : model->GetMaterialNames())
						{
							size_t fails = 0;
							std::string finalPath;

							do
							{
								finalPath = Tools::Tools_PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

								++fails;
							} while (std::filesystem::exists(finalPath));

							{
								std::ofstream outfile(finalPath);
								outfile << "<root><shader>:Shaders\\Unlit.glsl</shader></root>" << std::endl;
							}

							DuplicateEvent.Invoke(finalPath);
						}
					}
				};

			generateMaterialsMenu.CreateWidget<UI::UI_MenuItem>("Lambert").mClickedEvent += [this]
				{
					auto& modelManager = OVSERVICE(Core::Core_ModelManager);
					std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
					if (auto model = modelManager.GetResource(resourcePath))
					{
						for (const std::string& materialName : model->GetMaterialNames())
						{
							size_t fails = 0;
							std::string finalPath;

							do
							{
								finalPath = Tools::Tools_PathParser::GetContainingFolder(filePath) + (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".ovmat";

								++fails;
							} while (std::filesystem::exists(finalPath));

							{
								std::ofstream outfile(finalPath);
								outfile << "<root><shader>:Shaders\\Lambert.glsl</shader></root>" << std::endl;
							}

							DuplicateEvent.Invoke(finalPath);
						}
					}
				};
		}

		Editor_PreviewableContextualMenu::CreateList();
	}
};

class Editor_TextureContextualMenu : public Editor_PreviewableContextualMenu<Render::Render_Texture, Core::Core_TextureManager>
{
public:
	Editor_TextureContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& reloadAction = CreateWidget<UI::UI_MenuItem>("Reload");

		reloadAction.mClickedEvent += [this]
			{
				auto& textureManager = OVSERVICE(Core::Core_TextureManager);
				std::string resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				if (textureManager.IsResourceRegistered(resourcePath))
				{
					textureManager.Core_AResourceManager::ReloadResource(resourcePath);
					EDITOR_PANEL(Editor::Editor_MaterialEditor, "Material Editor").Refresh();
				}
			};

		Editor_PreviewableContextualMenu::CreateList();
	}
};

class Editor_SceneContextualMenu : public Editor_FileContextualMenu
{
public:
	Editor_SceneContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_FileContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<UI::UI_MenuItem>("Edit");

		editAction.mClickedEvent += [this]
			{
				EDITOR_EXEC(LoadSceneFromDisk(EDITOR_EXEC(GetResourcePath(filePath))));
			};

		Editor_FileContextualMenu::CreateList();
	}
};

class Editor_MaterialContextualMenu : public Editor_PreviewableContextualMenu<Core::Core_Material, Core::Core_MaterialManager>
{
public:
	Editor_MaterialContextualMenu(const std::string& p_filePath, bool p_protected = false) : Editor_PreviewableContextualMenu(p_filePath, p_protected) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<UI::UI_MenuItem>("Edit");

		editAction.mClickedEvent += [this]
			{
				Core::Core_Material* material = OVSERVICE(Core::Core_MaterialManager)[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
				if (material)
				{
					auto& materialEditor = EDITOR_PANEL(Editor::Editor_MaterialEditor, "Material Editor");
					materialEditor.SetTarget(*material);
					materialEditor.Open();
					materialEditor.Focus();

					Core::Core_Material* resource = Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>()[EDITOR_EXEC(GetResourcePath(filePath, m_protected))];
					auto& assetView = EDITOR_PANEL(Editor::Editor_AssetView, "Asset View");
					assetView.SetResource(resource);
					assetView.Open();
					assetView.Focus();
				}
			};

		auto& reload = CreateWidget<UI::UI_MenuItem>("Reload");
		reload.mClickedEvent += [this]
			{
				auto materialManager = OVSERVICE(Core::Core_MaterialManager);
				auto resourcePath = EDITOR_EXEC(GetResourcePath(filePath, m_protected));
				Core::Core_Material* material = materialManager[resourcePath];
				if (material)
				{
					materialManager.Core_AResourceManager::ReloadResource(resourcePath);
					EDITOR_PANEL(Editor::Editor_MaterialEditor, "Material Editor").Refresh();
				}
			};

		Editor_PreviewableContextualMenu::CreateList();
	}
};

Editor::Editor_AssetBrowser::Editor_AssetBrowser(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings, const std::string& p_engineAssetFolder, const std::string& p_projectAssetFolder, const std::string& p_projectScriptFolder) :
	UI::UI_PanelWindow(p_title, p_opened, p_windowSettings),
	m_engineAssetFolder(p_engineAssetFolder),
	m_projectAssetFolder(p_projectAssetFolder),
	m_projectScriptFolder(p_projectScriptFolder)
{
	if (!std::filesystem::exists(m_projectAssetFolder))
	{
		std::filesystem::create_directories(m_projectAssetFolder);

		Window::Window_MessageBox message
		(
			"Assets folder not found",
			"The \"Assets/\" folders hasn't been found in your project directory.\nIt has been automatically generated",
			Window::Window_MessageBox::EMessageType::WARNING,
			Window::Window_MessageBox::EButtonLayout::OK
		);
	}

	if (!std::filesystem::exists(m_projectScriptFolder))
	{
		std::filesystem::create_directories(m_projectScriptFolder);

		Window::Window_MessageBox message
		(
			"Scripts folder not found",
			"The \"Scripts/\" folders hasn't been found in your project directory.\nIt has been automatically generated",
			Window::Window_MessageBox::EMessageType::WARNING,
			Window::Window_MessageBox::EButtonLayout::OK
		);
	}

	auto& refreshButton = CreateWidget<UI::UI_Button>("Rescan assets");
	refreshButton.mClickedEvent += std::bind(&Editor_AssetBrowser::Refresh, this);
	refreshButton.mLineBreak = false;
	refreshButton.mIdleBackgroundColor = { 0.f, 0.5f, 0.0f, 1.f };

	auto& importButton = CreateWidget<UI::UI_Button>("Import asset");
	importButton.mClickedEvent += EDITOR_BIND(ImportAsset, m_projectAssetFolder);
	importButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };

	m_assetList = &CreateWidget<UI::UI_Group>();

	Fill();
}

void Editor::Editor_AssetBrowser::Fill()
{
	m_assetList->CreateWidget<UI::UI_Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_engineAssetFolder), true);
	m_assetList->CreateWidget<UI::UI_Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectAssetFolder), false);
	m_assetList->CreateWidget<UI::UI_Separator>();
	ConsiderItem(nullptr, std::filesystem::directory_entry(m_projectScriptFolder), false, false, true);
}

void Editor::Editor_AssetBrowser::Clear()
{
	m_assetList->RemoveAllWidgets();
}

void Editor::Editor_AssetBrowser::Refresh()
{
	Clear();
	Fill();
}

void Editor::Editor_AssetBrowser::ParseFolder(UI::UI_TreeNode& pRoot, const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder)
{
	for (auto& item : std::filesystem::directory_iterator(p_directory))
	{
		if (item.is_directory())
		{
			ConsiderItem(&pRoot, item, p_isEngineItem, false, p_scriptFolder);
		}
	}

	for (auto& item : std::filesystem::directory_iterator(p_directory))
	{
		if (!item.is_directory())
		{
			ConsiderItem(&pRoot, item, p_isEngineItem, false, p_scriptFolder);
		}
	}
}

void Editor::Editor_AssetBrowser::ConsiderItem(UI::UI_TreeNode* pRoot, const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen, bool p_scriptFolder)
{
	bool isDirectory = p_entry.is_directory();
	std::string itemname = Tools::Tools_PathParser::GetElementName(p_entry.path().string());
	std::string path = p_entry.path().string();
	if (isDirectory && path.back() != '\\')
	{
		path += '\\';
	}
	std::string resourceFormatPath = EDITOR_EXEC(GetResourcePath(path, p_isEngineItem));
	bool protectedItem = !pRoot || p_isEngineItem;

	Tools::Tools_PathParser::EFileType fileType = Tools::Tools_PathParser::GetFileType(itemname);

	if (fileType == Tools::Tools_PathParser::EFileType::UNKNOWN && !isDirectory)
	{
		return;
	}

	auto& itemGroup = pRoot ? pRoot->CreateWidget<UI::UI_Group>() : m_assetList->CreateWidget<UI::UI_Group>();

	uint32_t iconTextureID = isDirectory ? EDITOR_CONTEXT(editorResources)->GetTexture("Icon_Folder")->id : EDITOR_CONTEXT(editorResources)->GetFileIcon(itemname)->id;

	itemGroup.CreateWidget<UI::UI_Image>(iconTextureID, glm::vec2{ 16, 16 }).mLineBreak = false;

	if (isDirectory)
	{
		auto& treeNode = itemGroup.CreateWidget<UI::UI_TreeNode>(itemname);

		if (p_autoOpen)
		{
			treeNode.Open();
		}

		auto& ddSource = treeNode.AddPlugin<UI::UI_DDSource<std::pair<std::string, UI::UI_Group*>>>("Folder", resourceFormatPath, std::make_pair(resourceFormatPath, &itemGroup));

		if (!pRoot || p_scriptFolder)
		{
			treeNode.RemoveAllPlugins();
		}

		auto& contextMenu = !p_scriptFolder ? treeNode.AddPlugin<Editor_FolderContextualMenu>(path, protectedItem && resourceFormatPath != "") : treeNode.AddPlugin<Editor_ScriptFolderContextualMenu>(path, protectedItem && resourceFormatPath != "");
		contextMenu.userData = static_cast<void*>(&treeNode);

		contextMenu.ItemAddedEvent += [this, &treeNode, path, p_isEngineItem, p_scriptFolder](std::string p_string)
			{
				treeNode.Open();
				treeNode.RemoveAllWidgets();
				ParseFolder(treeNode, std::filesystem::directory_entry(Tools::Tools_PathParser::GetContainingFolder(p_string)), p_isEngineItem, p_scriptFolder);
			};

		if (!p_scriptFolder)
		{
			if (!p_isEngineItem)
			{
				treeNode.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("Folder").mDataReceivedEvent += [this, &treeNode, path, p_isEngineItem](std::pair<std::string, UI::UI_Group*> p_data)
					{
						if (!p_data.first.empty())
						{
							std::string folderReceivedPath = EDITOR_EXEC(GetRealPath(p_data.first));

							std::string folderName = Tools::Tools_PathParser::GetElementName(folderReceivedPath) + '\\';
							std::string prevPath = folderReceivedPath;
							std::string correctPath = m_pathUpdate.find(&treeNode) != m_pathUpdate.end() ? m_pathUpdate.at(&treeNode) : path;
							std::string newPath = correctPath + folderName;

							if (!(newPath.find(prevPath) != std::string::npos) || prevPath == newPath)
							{
								if (!std::filesystem::exists(newPath))
								{
									bool isEngineFolder = p_data.first.at(0) == ':';

									if (isEngineFolder)
									{
										std::filesystem::copy(prevPath, newPath, std::filesystem::copy_options::recursive);
									}
									else
									{
										RenameAsset(prevPath, newPath);
										EDITOR_EXEC(PropagateFolderRename(prevPath, newPath));
									}

									treeNode.Open();
									treeNode.RemoveAllWidgets();
									ParseFolder(treeNode, std::filesystem::directory_entry(correctPath), p_isEngineItem);

									if (!isEngineFolder)
										p_data.second->Destroy();
								}
								else if (prevPath == newPath)
								{
									// Ignore
								}
								else
								{
									Window::Window_MessageBox errorMessage("Folder already exists", "You can't move this folder to this location because the name is already taken", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK);
								}
							}
							else
							{
								Window::Window_MessageBox errorMessage("Wow!", "Crazy boy!", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK);
							}
						}
					};

				treeNode.AddPlugin<UI::UI_DDTarget<std::pair<std::string, UI::UI_Group*>>>("File").mDataReceivedEvent += [this, &treeNode, path, p_isEngineItem](std::pair<std::string, UI::UI_Group*> p_data)
					{
						if (!p_data.first.empty())
						{
							std::string fileReceivedPath = EDITOR_EXEC(GetRealPath(p_data.first));

							std::string fileName = Tools::Tools_PathParser::GetElementName(fileReceivedPath);
							std::string prevPath = fileReceivedPath;
							std::string correctPath = m_pathUpdate.find(&treeNode) != m_pathUpdate.end() ? m_pathUpdate.at(&treeNode) : path;
							std::string newPath = correctPath + fileName;

							if (!std::filesystem::exists(newPath))
							{
								bool isEngineFile = p_data.first.at(0) == ':';

								if (isEngineFile)
								{
									std::filesystem::copy_file(prevPath, newPath);
								}
								else
								{
									RenameAsset(prevPath, newPath);
									EDITOR_EXEC(PropagateFileRename(prevPath, newPath));
								}

								treeNode.Open();
								treeNode.RemoveAllWidgets();
								ParseFolder(treeNode, std::filesystem::directory_entry(correctPath), p_isEngineItem);

								if (!isEngineFile)
									p_data.second->Destroy();
							}
							else if (prevPath == newPath)
							{
								// Ignore
							}
							else
							{
								Window::Window_MessageBox errorMessage("File already exists", "You can't move this file to this location because the name is already taken", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK);
							}
						}
					};
			}

			contextMenu.DestroyedEvent += [&itemGroup](std::string p_deletedPath) { itemGroup.Destroy(); };

			contextMenu.RenamedEvent += [this, &treeNode, path, &ddSource, p_isEngineItem](std::string p_prev, std::string p_newPath)
				{
					p_newPath += '\\';

					if (!std::filesystem::exists(p_newPath))
					{
						RenameAsset(p_prev, p_newPath);
						EDITOR_EXEC(PropagateFolderRename(p_prev, p_newPath));
						std::string elementName = Tools::Tools_PathParser::GetElementName(p_newPath);
						std::string data = Tools::Tools_PathParser::GetContainingFolder(ddSource.mData.first) + elementName + "\\";
						ddSource.mData.first = data;
						ddSource.mTooltip = data;
						treeNode.mName = elementName;
						treeNode.Open();
						treeNode.RemoveAllWidgets();
						ParseFolder(treeNode, std::filesystem::directory_entry(p_newPath), p_isEngineItem);
						m_pathUpdate[&treeNode] = p_newPath;
					}
					else
					{
						Window::Window_MessageBox errorMessage("Folder already exists", "You can't rename this folder because the given name is already taken", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK);
					}
				};

			contextMenu.ItemAddedEvent += [this, &treeNode, p_isEngineItem](std::string p_path)
				{
					treeNode.RemoveAllWidgets();
					ParseFolder(treeNode, std::filesystem::directory_entry(Tools::Tools_PathParser::GetContainingFolder(p_path)), p_isEngineItem);
				};

		}

		contextMenu.CreateList();

		treeNode.mOpenedEvent += [this, &treeNode, path, p_isEngineItem, p_scriptFolder]
			{
				treeNode.RemoveAllWidgets();
				std::string updatedPath = Tools::Tools_PathParser::GetContainingFolder(path) + treeNode.mName;
				ParseFolder(treeNode, std::filesystem::directory_entry(updatedPath), p_isEngineItem, p_scriptFolder);
			};

		treeNode.mClosedEvent += [this, &treeNode]
			{
				treeNode.RemoveAllWidgets();
			};
	}
	else
	{
		auto& clickableText = itemGroup.CreateWidget<UI::UI_TextClickable>(itemname);

		Editor_FileContextualMenu* contextMenu = nullptr;

		switch (fileType)
		{
			case Tools::Tools_PathParser::EFileType::MODEL:		contextMenu = &clickableText.AddPlugin<Editor_ModelContextualMenu>(path, protectedItem);	break;
			case Tools::Tools_PathParser::EFileType::TEXTURE:	contextMenu = &clickableText.AddPlugin<Editor_TextureContextualMenu>(path, protectedItem); 	break;
			case Tools::Tools_PathParser::EFileType::SHADER:	contextMenu = &clickableText.AddPlugin<Editor_ShaderContextualMenu>(path, protectedItem);	break;
			case Tools::Tools_PathParser::EFileType::MATERIAL:	contextMenu = &clickableText.AddPlugin<Editor_MaterialContextualMenu>(path, protectedItem);	break;
			case Tools::Tools_PathParser::EFileType::SCENE:		contextMenu = &clickableText.AddPlugin<Editor_SceneContextualMenu>(path, protectedItem);	break;
			default: contextMenu = &clickableText.AddPlugin<Editor_FileContextualMenu>(path, protectedItem); break;
		}

		contextMenu->CreateList();

		contextMenu->DestroyedEvent += [&itemGroup](std::string p_deletedPath)
			{
				itemGroup.Destroy();

				if (EDITOR_CONTEXT(sceneManager).GetCurrentSceneSourcePath() == p_deletedPath)
				{
					EDITOR_CONTEXT(sceneManager).ForgetCurrentSceneSourcePath();
				}
			};

		auto& ddSource = clickableText.AddPlugin<UI::UI_DDSource<std::pair<std::string, UI::UI_Group*>>>
			(
				"File",
				resourceFormatPath,
				std::make_pair(resourceFormatPath, &itemGroup)
			);

		contextMenu->RenamedEvent += [&ddSource, &clickableText, p_scriptFolder](std::string p_prev, std::string p_newPath)
			{
				if (p_newPath != p_prev)
				{
					if (!std::filesystem::exists(p_newPath))
					{
						RenameAsset(p_prev, p_newPath);
						std::string elementName = Tools::Tools_PathParser::GetElementName(p_newPath);
						ddSource.mData.first = Tools::Tools_PathParser::GetContainingFolder(ddSource.mData.first) + elementName;

						if (!p_scriptFolder)
						{
							EDITOR_EXEC(PropagateFileRename(p_prev, p_newPath));
							if (EDITOR_CONTEXT(sceneManager).GetCurrentSceneSourcePath() == p_prev)
							{
								EDITOR_CONTEXT(sceneManager).StoreCurrentSceneSourcePath(p_newPath);
							}
						}
						else
						{
							EDITOR_EXEC(PropagateScriptRename(p_prev, p_newPath));
						}

						clickableText.mContent = elementName;
					}
					else
					{
						Window::Window_MessageBox errorMessage("File already exists", "You can't rename this file because the given name is already taken", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK);
					}
				}
			};

		contextMenu->DuplicateEvent += [this, &clickableText, pRoot, path, p_isEngineItem](std::string newItem)
			{
				EDITOR_EXEC(DelayAction(std::bind(&Editor_AssetBrowser::ConsiderItem, this, pRoot, std::filesystem::directory_entry{ newItem }, p_isEngineItem, false, false), 0));
			};

		if (fileType == Tools::Tools_PathParser::EFileType::TEXTURE)
		{
			auto& texturePreview = clickableText.AddPlugin<Editor_TexturePreview>();
			texturePreview.SetPath(resourceFormatPath);
		}
	}
}

