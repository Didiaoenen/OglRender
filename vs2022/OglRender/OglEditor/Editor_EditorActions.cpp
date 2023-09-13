#include <fstream>
#include <iostream>
#include <filesystem>

#include <OglCore/Core_CModelRenderer.h>
#include <OglCore/Core_CMaterialRenderer.h>

#include <OglWindow/Window_OpenFileDialog.h>
#include <OglWindow/Window_SaveFileDialog.h>
#include <OglWindow/Window_MessageBox.h>

#include <OglTools/Tools_String.h>
#include <OglTools/Tools_PathParser.h>
#include <OglTools/Tools_SystemCalls.h>

#include "Editor_GameView.h"
#include "Editor_SceneView.h"
#include "Editor_AssetView.h"
#include "Editor_Inspector.h"
#include "Editor_EditorActions.h"
#include "Editor_ProjectSettings.h"
#include "Editor_MaterialEditor.h"

#include "Editor_EditorActions.h"

Editor::Editor_EditorActions::Editor_EditorActions(Editor_Context& p_context, Editor_EditorRenderer& p_editorRenderer, Editor_PanelsManager& p_panelsManager) :
	mContext(p_context),
	mRenderer(p_editorRenderer),
	m_panelsManager(p_panelsManager)
{
	Core::Core_ServiceLocator::Provide<Editor_EditorActions>(*this);

	mContext.sceneManager.CurrentSceneSourcePathChangedEvent += [this](const std::string& p_newPath)
		{
			std::string titleExtra = " - " + (p_newPath.empty() ? "Untitled Scene" : GetResourcePath(p_newPath));
			mContext.window->SetTitle(mContext.windowSettings.title + titleExtra);
		};
}

Editor::Editor_Context& Editor::Editor_EditorActions::GetContext()
{
	return mContext;
}

Editor::Editor_EditorRenderer& Editor::Editor_EditorActions::GetRenderer()
{
	return mRenderer;
}

Editor::Editor_PanelsManager& Editor::Editor_EditorActions::GetPanelsManager()
{
	return m_panelsManager;
}

void Editor::Editor_EditorActions::SetActorSpawnAtOrigin(bool pValue)
{
	if (pValue)
	{
		m_actorSpawnMode = EActorSpawnMode::ORIGIN;
	}
	else
	{
		m_actorSpawnMode = EActorSpawnMode::FRONT;
	}
}

void Editor::Editor_EditorActions::SetActorSpawnMode(EActorSpawnMode pValue)
{
	m_actorSpawnMode = pValue;
}

void Editor::Editor_EditorActions::ResetLayout()
{
	DelayAction([this]() {mContext.uiManager->ResetLayout("Config\\layout.ini"); });
}

void Editor::Editor_EditorActions::SetSceneViewCameraSpeed(int p_speed)
{
	EDITOR_PANEL(Editor_SceneView, "Scene View").GetCameraController().SetSpeed((float)p_speed);
}

int Editor::Editor_EditorActions::GetSceneViewCameraSpeed()
{
	return (int)EDITOR_PANEL(Editor_SceneView, "Scene View").GetCameraController().GetSpeed();
}

void Editor::Editor_EditorActions::SetAssetViewCameraSpeed(int p_speed)
{
	EDITOR_PANEL(Editor_AssetView, "Asset View").GetCameraController().SetSpeed((float)p_speed);
}

int Editor::Editor_EditorActions::GetAssetViewCameraSpeed()
{
	return (int)EDITOR_PANEL(Editor_AssetView, "Asset View").GetCameraController().GetSpeed();
}

void Editor::Editor_EditorActions::ResetSceneViewCameraPosition()
{
	EDITOR_PANEL(Editor_SceneView, "Scene View").GetCameraController().SetPosition({ -10.0f, 4.0f, 10.0f });
}

void Editor::Editor_EditorActions::ResetAssetViewCameraPosition()
{
	EDITOR_PANEL(Editor_AssetView, "Asset View").GetCameraController().SetPosition({ -10.0f, 4.0f, 10.0f });
}

Editor::Editor_EditorActions::EEditorMode Editor::Editor_EditorActions::GetCurrentEditorMode() const
{
	return m_editorMode;
}

void Editor::Editor_EditorActions::SetEditorMode(EEditorMode p_newEditorMode)
{
	m_editorMode = p_newEditorMode;
	EditorModeChangedEvent.Invoke(m_editorMode);
}

void Editor::Editor_EditorActions::StartPlaying()
{
	if (m_editorMode == EEditorMode::EDIT)
	{
		//mContext.scriptInterpreter->RefreshAll();
		EDITOR_PANEL(Editor_Inspector, "Inspector").Refresh();

		//if (mContext.scriptInterpreter->IsOk())
		//{
		//	PlayEvent.Invoke();
		//	m_sceneBackup.Clear();
		//	tinyxml2::XMLNode* node = m_sceneBackup.NewElement("root");
		//	m_sceneBackup.InsertFirstChild(node);
		//	mContext.sceneManager.GetCurrentScene()->OnSerialize(m_sceneBackup, node);
		//	m_panelsManager.GetPanelAs<Editor_GameView>("Game View").Focus();
		//	mContext.sceneManager.GetCurrentScene()->Play();
		//	SetEditorMode(EEditorMode::PLAY);
		//}
	}
	else
	{
		//mContext.audioEngine->Unsuspend();
		SetEditorMode(EEditorMode::PLAY);
	}
}

void Editor::Editor_EditorActions::PauseGame()
{
	SetEditorMode(EEditorMode::PAUSE);
	//mContext.audioEngine->Suspend();
}

void Editor::Editor_EditorActions::StopPlaying()
{
	if (m_editorMode != EEditorMode::EDIT)
	{
		ImGui::GetIO().DisableMouseUpdate = false;
		mContext.window->SetCursorMode(Window::ECursorMode::NORMAL);
		SetEditorMode(EEditorMode::EDIT);
		bool loadedFromDisk = mContext.sceneManager.IsCurrentSceneLoadedFromDisk();
		std::string sceneSourcePath = mContext.sceneManager.GetCurrentSceneSourcePath();

		int64_t focusedActorID = -1;

		if (auto targetActor = EDITOR_PANEL(Editor_Inspector, "Inspector").GetTargetActor())
			focusedActorID = targetActor->GetID();

		mContext.sceneManager.LoadSceneFromMemory(m_sceneBackup);
		if (loadedFromDisk)
			mContext.sceneManager.StoreCurrentSceneSourcePath(sceneSourcePath);
		m_sceneBackup.Clear();
		EDITOR_PANEL(Editor_SceneView, "Scene View").Focus();
		if (auto actorInstance = mContext.sceneManager.GetCurrentScene()->FindActorByID(focusedActorID))
		{
			EDITOR_PANEL(Editor_Inspector, "Inspector").FocusActor(*actorInstance);
		}
	}
}

void Editor::Editor_EditorActions::NextFrame()
{
	if (m_editorMode == EEditorMode::PLAY || m_editorMode == EEditorMode::PAUSE)
	{
		SetEditorMode(EEditorMode::FRAME_BY_FRAME);
	}
}

glm::vec3 Editor::Editor_EditorActions::CalculateActorSpawnPoint(float p_distanceToCamera)
{
	auto& sceneView = m_panelsManager.GetPanelAs<Editor_SceneView>("Scene View");
	return sceneView.GetCameraPosition() + sceneView.GetCameraRotation() * glm::vec3(0.f, 0.f, 1.f) * p_distanceToCamera;
}

Core::Core_Actor& Editor::Editor_EditorActions::CreateEmptyActor(bool p_focusOnCreation, Core::Core_Actor* pParent, const std::string& pName)
{
	const auto currentScene = mContext.sceneManager.GetCurrentScene();
	auto& instance = pName.empty() ? currentScene->CreateActor() : currentScene->CreateActor(pName);

	if (pParent)
	{
		instance.SetParent(*pParent);
	}

	if (m_actorSpawnMode == EActorSpawnMode::FRONT)
	{
		instance.transform.SetLocalPosition(CalculateActorSpawnPoint(10.0f));
	}

	if (p_focusOnCreation)
	{
		SelectActor(instance);
	}

	//OVLOG_INFO("Actor created");

	return instance;
}

Core::Core_Actor& Editor::Editor_EditorActions::CreateActorWithModel(const std::string& pPath, bool p_focusOnCreation, Core::Core_Actor* pParent, const std::string& pName)
{
	auto& instance = CreateEmptyActor(false, pParent, pName);

	auto& modelRenderer = instance.AddComponent<Core::Core_CModelRenderer>();

	const auto model = mContext.modelManager[pPath];
	if (model)
	{
		modelRenderer.SetModel(model);
	}

	auto& materialRenderer = instance.AddComponent<Core::Core_CMaterialRenderer>();
	const auto material = mContext.materialManager[":Materials\\Default.ovmat"];
	if (material)
		materialRenderer.FillWithMaterial(*material);

	if (p_focusOnCreation)
		SelectActor(instance);

	return instance;
}

bool Editor::Editor_EditorActions::DestroyActor(Core::Core_Actor& p_actor)
{
	p_actor.MarkAsDestroy();
	//OVLOG_INFO("Actor destroyed");
	return true;
}

std::string FindDuplicatedActorUniqueName(Core::Core_Actor& p_duplicated, Core::Core_Actor& p_newActor, Core::Core_Scene& p_scene)
{
	const auto parent = p_newActor.GetParent();
	const auto adjacentActors = parent ? parent->GetChildren() : p_scene.GetActors();

	auto availabilityChecker = [&parent, &adjacentActors](std::string target) -> bool
		{
			const auto isActorNameTaken = [&target, parent](auto actor) { return (parent || !actor->GetParent()) && actor->GetName() == target; };
			return std::find_if(adjacentActors.begin(), adjacentActors.end(), isActorNameTaken) == adjacentActors.end();
		};

	return Tools::Tools_String::GenerateUnique(p_duplicated.GetName(), availabilityChecker);
}

void Editor::Editor_EditorActions::DuplicateActor(Core::Core_Actor& p_toDuplicate, Core::Core_Actor* p_forcedParent, bool p_focus)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* actorsRoot = doc.NewElement("actors");
	p_toDuplicate.OnSerialize(doc, actorsRoot);
	auto& newActor = CreateEmptyActor(false);
	int64_t idToUse = newActor.GetID();
	tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");
	newActor.OnDeserialize(doc, currentActor);

	newActor.SetID(idToUse);

	if (p_forcedParent)
		newActor.SetParent(*p_forcedParent);
	else
	{
		auto currentScene = mContext.sceneManager.GetCurrentScene();

		if (newActor.GetParentID() > 0)
		{
			if (auto found = currentScene->FindActorByID(newActor.GetParentID()); found)
			{
				newActor.SetParent(*found);
			}
		}

		const auto uniqueName = FindDuplicatedActorUniqueName(p_toDuplicate, newActor, *currentScene);
		newActor.SetName(uniqueName);
	}

	if (p_focus)
	{
		SelectActor(newActor);
	}

	for (auto& child : p_toDuplicate.GetChildren())
	{
		DuplicateActor(*child, &newActor, false);
	}
}

void Editor::Editor_EditorActions::SelectActor(Core::Core_Actor& p_target)
{
	EDITOR_PANEL(Editor_Inspector, "Inspector").FocusActor(p_target);
}

void Editor::Editor_EditorActions::UnselectActor()
{
	EDITOR_PANEL(Editor_Inspector, "Inspector").UnFocus();
}

bool Editor::Editor_EditorActions::IsAnyActorSelected() const
{
	return EDITOR_PANEL(Editor_Inspector, "Inspector").GetTargetActor();
}

Core::Core_Actor& Editor::Editor_EditorActions::GetSelectedActor() const
{
	return *EDITOR_PANEL(Editor_Inspector, "Inspector").GetTargetActor();
}

void Editor::Editor_EditorActions::MoveToTarget(Core::Core_Actor& p_target)
{
	EDITOR_PANEL(Editor_SceneView, "Scene View").GetCameraController().MoveToTarget(p_target);
}

void Editor::Editor_EditorActions::CompileShaders()
{
	for (auto shader : mContext.shaderManager.GetResources())
	{
		Render::Render_ShaderLoader::Recompile(*shader.second, GetRealPath(shader.second->path));
	}
}

void Editor::Editor_EditorActions::SaveMaterials()
{
	for (auto& [id, material] : mContext.materialManager.GetResources())
	{
		Core::Core_MaterialLoader::Save(*material, GetRealPath(material->path));
	}
}

bool Editor::Editor_EditorActions::ImportAsset(const std::string& p_initialDestinationDirectory)
{
	std::string modelFormats = "*.fbx;*.obj;";
	std::string textureFormats = "*.png;*.jpeg;*.jpg;*.tga";
	std::string shaderFormats = "*.glsl;";
	std::string soundFormats = "*.mp3;*.ogg;*.wav;";

	Window::Window_OpenFileDialog selectAssetDialog("Select an asset to import");
	selectAssetDialog.AddFileType("Any supported format", modelFormats + textureFormats + shaderFormats + soundFormats);
	selectAssetDialog.AddFileType("Model (.fbx, .obj)", modelFormats);
	selectAssetDialog.AddFileType("Texture (.png, .jpeg, .jpg, .tga)", textureFormats);
	selectAssetDialog.AddFileType("Shader (.glsl)", shaderFormats);
	selectAssetDialog.AddFileType("Sound (.mp3, .ogg, .wav)", soundFormats);
	selectAssetDialog.Show();

	if (selectAssetDialog.HasSucceeded())
	{
		std::string source = selectAssetDialog.GetSelectedFilePath();
		std::string extension = '.' + Tools::Tools_PathParser::GetExtension(source);
		std::string filename = selectAssetDialog.GetSelectedFileName();

		Window::Window_SaveFileDialog saveLocationDialog("Where to import?");
		saveLocationDialog.SetInitialDirectory(p_initialDestinationDirectory + filename);
		saveLocationDialog.DefineExtension(extension, extension);
		saveLocationDialog.Show();

		if (saveLocationDialog.HasSucceeded())
		{
			std::string destination = saveLocationDialog.GetSelectedFilePath();

			if (!std::filesystem::exists(destination) || Window::Window_MessageBox("File already exists", "The destination you have selected already exists, importing this file will erase the previous file content, are you sure about that?", Window::Window_MessageBox::EMessageType::WARNING, Window::Window_MessageBox::EButtonLayout::OK_CANCEL).GetUserAction() == Window::Window_MessageBox::EUserAction::OK)
			{
				std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
				//OVLOG_INFO("Asset \"" + destination + "\" imported");
				return true;
			}
		}
	}

	return false;
}

bool Editor::Editor_EditorActions::ImportAssetAtLocation(const std::string& p_destination)
{
	std::string modelFormats = "*.fbx;*.obj;";
	std::string textureFormats = "*.png;*.jpeg;*.jpg;*.tga;";
	std::string shaderFormats = "*.glsl;";
	std::string soundFormats = "*.mp3;*.ogg;*.wav;";

	Window::Window_OpenFileDialog selectAssetDialog("Select an asset to import");
	selectAssetDialog.AddFileType("Any supported format", modelFormats + textureFormats + shaderFormats + soundFormats);
	selectAssetDialog.AddFileType("Model (.fbx, .obj)", modelFormats);
	selectAssetDialog.AddFileType("Texture (.png, .jpeg, .jpg, .tga)", textureFormats);
	selectAssetDialog.AddFileType("Shader (.glsl)", shaderFormats);
	selectAssetDialog.AddFileType("Sound (.mp3, .ogg, .wav)", soundFormats);
	selectAssetDialog.Show();

	if (selectAssetDialog.HasSucceeded())
	{
		std::string source = selectAssetDialog.GetSelectedFilePath();
		std::string destination = p_destination + selectAssetDialog.GetSelectedFileName();

		if (!std::filesystem::exists(destination) || Window::Window_MessageBox("File already exists", "The destination you have selected already exists, importing this file will erase the previous file content, are you sure about that?", Window::Window_MessageBox::EMessageType::WARNING, Window::Window_MessageBox::EButtonLayout::OK_CANCEL).GetUserAction() == Window::Window_MessageBox::EUserAction::OK)
		{
			std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
			//OVLOG_INFO("Asset \"" + destination + "\" imported");
			return true;
		}
	}

	return false;
}

std::string Editor::Editor_EditorActions::GetRealPath(const std::string& pPath)
{
	std::string result;

	if (pPath[0] == ':')
	{
		result = mContext.mEngineAssetsPath + std::string(pPath.data() + 1, pPath.data() + pPath.size());
	}
	else
	{
		result = mContext.mProjectAssetsPath + pPath;
	}

	return result;
}

std::string Editor::Editor_EditorActions::GetResourcePath(const std::string& pPath, bool p_isFromEngine)
{
	std::string result = pPath;

	if (Tools::Tools_String::Replace(result, p_isFromEngine ? mContext.mEngineAssetsPath : mContext.mProjectAssetsPath, ""))
	{
		if (p_isFromEngine)
		{
			result = ':' + result;
		}
	}

	return result;
}

std::string Editor::Editor_EditorActions::GetScriptPath(const std::string& pPath)
{
	std::string result = pPath;

	Tools::Tools_String::Replace(result, mContext.mProjectScriptsPath, "");
	Tools::Tools_String::Replace(result, ".lua", "");

	return result;
}

void Editor::Editor_EditorActions::PropagateFolderRename(std::string p_previousName, std::string p_newName)
{
	p_previousName = Tools::Tools_PathParser::MakeNonWindowsStyle(p_previousName);
	p_newName = Tools::Tools_PathParser::MakeNonWindowsStyle(p_newName);

	for (auto& p : std::filesystem::recursive_directory_iterator(p_newName))
	{
		if (!p.is_directory())
		{
			std::string newFileName = Tools::Tools_PathParser::MakeNonWindowsStyle(p.path().string());
			std::string previousFileName;

			for (char c : newFileName)
			{
				previousFileName += c;
				if (previousFileName == p_newName)
				{
					previousFileName = p_previousName;
				}
			}

			PropagateFileRename(Tools::Tools_PathParser::MakeWindowsStyle(previousFileName), Tools::Tools_PathParser::MakeWindowsStyle(newFileName));
		}
	}
}

void Editor::Editor_EditorActions::PropagateFolderDestruction(std::string p_folderPath)
{
	for (auto& p : std::filesystem::recursive_directory_iterator(p_folderPath))
	{
		if (!p.is_directory())
		{
			PropagateFileRename(Tools::Tools_PathParser::MakeWindowsStyle(p.path().string()), "?");
		}
	}
}

void Editor::Editor_EditorActions::PropagateScriptRename(std::string p_previousName, std::string p_newName)
{
	p_previousName = GetScriptPath(p_previousName);
	p_newName = GetScriptPath(p_newName);

	if (auto currentScene = mContext.sceneManager.GetCurrentScene())
	{
		for (auto actor : currentScene->GetActors())
		{
			if (actor->RemoveBehaviour(p_previousName))
			{
				actor->AddBehaviour(p_newName);
			}
		}
	}

	PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Tools_PathParser::EFileType::SCENE);

	EDITOR_PANEL(Editor_Inspector, "Inspector").Refresh();
}

void Editor::Editor_EditorActions::PropagateFileRename(std::string p_previousName, std::string p_newName)
{
	p_previousName = GetResourcePath(p_previousName);
	p_newName = GetResourcePath(p_newName);

	if (p_newName != "?")
	{
		if (Core::Core_ServiceLocator::Get<Core::Core_ModelManager>().MoveResource(p_previousName, p_newName))
		{
			Render::Render_Model* resource = Core::Core_ServiceLocator::Get<Core::Core_ModelManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(Render::Render_Model, path)) = p_newName;
		}

		if (Core::Core_ServiceLocator::Get<Core::Core_TextureManager>().MoveResource(p_previousName, p_newName))
		{
			Render::Render_Texture* resource = Core::Core_ServiceLocator::Get<Core::Core_TextureManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(Render::Render_Texture, path)) = p_newName;
		}

		if (Core::Core_ServiceLocator::Get<Core::Core_ShaderManager>().MoveResource(p_previousName, p_newName))
		{
			Render::Render_Shader* resource = Core::Core_ServiceLocator::Get<Core::Core_ShaderManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(Render::Render_Shader, path)) = p_newName;
		}

		if (Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>().MoveResource(p_previousName, p_newName))
		{
			Core::Core_Material* resource = Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(Core::Core_Material, path)) = p_newName;
		}
	}
	else
	{
		if (auto texture = Core::Core_ServiceLocator::Get<Core::Core_TextureManager>().GetResource(p_previousName, false))
		{
			for (auto [name, instance] : Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>().GetResources())
			{
				if (instance)
				{
					for (auto& [name, value] : instance->GetUniformsData())
					{
						if (value.has_value() && value.type() == typeid(Render::Render_Texture*))
						{
							if (std::any_cast<Render::Render_Texture*>(value) == texture)
							{
								value = static_cast<Render::Render_Texture*>(nullptr);
							}
						}
					}
				}
			}

			auto& assetView = EDITOR_PANEL(Editor_AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<Render::Render_Texture*>(&assetViewRes); pval && *pval)
			{
				assetView.SetResource(static_cast<Render::Render_Texture*>(nullptr));
			}

			Core::Core_ServiceLocator::Get<Core::Core_TextureManager>().UnloadResource(p_previousName);
		}

		if (auto shader = Core::Core_ServiceLocator::Get<Core::Core_ShaderManager>().GetResource(p_previousName, false))
		{
			auto& materialEditor = EDITOR_PANEL(Editor_MaterialEditor, "Material Editor");

			for (auto [name, instance] : Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>().GetResources())
			{
				if (instance && instance->GetShader() == shader)
				{
					instance->SetShader(nullptr);
				}
			}

			Core::Core_ServiceLocator::Get<Core::Core_ShaderManager>().UnloadResource(p_previousName);
		}

		if (auto model = Core::Core_ServiceLocator::Get<Core::Core_ModelManager>().GetResource(p_previousName, false))
		{
			auto& assetView = EDITOR_PANEL(Editor_AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<Render::Render_Model*>(&assetViewRes); pval && *pval)
			{
				assetView.SetResource(static_cast<Render::Render_Model*>(nullptr));
			}

			if (auto currentScene = mContext.sceneManager.GetCurrentScene())
			{
				for (auto actor : currentScene->GetActors())
				{
					if (auto modelRenderer = actor->GetComponent<Core::Core_CModelRenderer>(); modelRenderer && modelRenderer->GetModel() == model)
					{
						modelRenderer->SetModel(nullptr);
					}
				}
			}

			Core::Core_ServiceLocator::Get<Core::Core_ModelManager>().UnloadResource(p_previousName);
		}

		if (auto material = Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>().GetResource(p_previousName, false))
		{
			auto& materialEditor = EDITOR_PANEL(Editor_MaterialEditor, "Material Editor");

			if (materialEditor.GetTarget() == material)
				materialEditor.RemoveTarget();

			auto& assetView = EDITOR_PANEL(Editor_AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<Core::Core_Material*>(&assetViewRes); pval && *pval)
			{
				assetView.SetResource(static_cast<Core::Core_Material*>(nullptr));
			}

			if (auto currentScene = mContext.sceneManager.GetCurrentScene())
			{
				for (auto actor : currentScene->GetActors())
				{
					if (auto materialRenderer = actor->GetComponent<Core::Core_CMaterialRenderer>(); materialRenderer)
					{
						materialRenderer->RemoveMaterialByInstance(*material);
					}
				}
			}

			Core::Core_ServiceLocator::Get<Core::Core_MaterialManager>().UnloadResource(p_previousName);
		}
	}

	switch (Tools::Tools_PathParser::GetFileType(p_previousName))
	{
	case Tools::Tools_PathParser::EFileType::MATERIAL:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Tools_PathParser::EFileType::SCENE);
		break;
	case Tools::Tools_PathParser::EFileType::MODEL:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Tools_PathParser::EFileType::SCENE);
		break;
	case Tools::Tools_PathParser::EFileType::SHADER:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Tools_PathParser::EFileType::MATERIAL);
		break;
	case Tools::Tools_PathParser::EFileType::TEXTURE:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Tools_PathParser::EFileType::MATERIAL);
		break;
	case Tools::Tools_PathParser::EFileType::SOUND:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Tools_PathParser::EFileType::SCENE);
		break;
	}

	EDITOR_PANEL(Editor_Inspector, "Inspector").Refresh();
	EDITOR_PANEL(Editor_MaterialEditor, "Material Editor").Refresh();
}

void Editor::Editor_EditorActions::PropagateFileRenameThroughSavedFilesOfType(const std::string& p_previousName, const std::string& p_newName, Tools::Tools_PathParser::EFileType pFileType)
{
	for (auto& entry : std::filesystem::recursive_directory_iterator(mContext.mProjectAssetsPath))
	{
		if (Tools::Tools_PathParser::GetFileType(entry.path().string()) == pFileType)
		{
			using namespace std;

			{
				ifstream in(entry.path().string().c_str());
				ofstream out("TEMP");
				string wordToReplace(">" + p_previousName + "<");
				string wordToReplaceWith(">" + p_newName + "<");

				string line;
				size_t len = wordToReplace.length();
				while (getline(in, line))
				{
					if (Tools::Tools_String::Replace(line, wordToReplace, wordToReplaceWith))
					{
						//OVLOG_INFO("Asset retargeting: \"" + p_previousName + "\" to \"" + p_newName + "\" in \"" + entry.path().string() + "\"");
					}
					out << line << '\n';
				}

				out.close(); in.close();
			}

			std::filesystem::copy_file("TEMP", entry.path(), std::filesystem::copy_options::overwrite_existing);
			std::filesystem::remove("TEMP");
		}
	}
}

void Editor::Editor_EditorActions::LoadEmptyScene()
{
	if (GetCurrentEditorMode() != EEditorMode::EDIT)
	{
		StopPlaying();
	}

	mContext.sceneManager.LoadEmptyLightedScene();
	//OVLOG_INFO("New scene created");
}

void Editor::Editor_EditorActions::SaveCurrentSceneTo(const std::string& pPath)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* node = doc.NewElement("root");
	doc.InsertFirstChild(node);
	mContext.sceneManager.StoreCurrentSceneSourcePath(pPath);
	mContext.sceneManager.GetCurrentScene()->OnSerialize(doc, node);
	doc.SaveFile(pPath.c_str());
}

void Editor::Editor_EditorActions::LoadSceneFromDisk(const std::string& pPath, bool p_absolute)
{
	if (GetCurrentEditorMode() != EEditorMode::EDIT)
	{
		StopPlaying();
	}

	mContext.sceneManager.LoadScene(pPath, p_absolute);
	//OVLOG_INFO("Scene loaded from disk: " + mContext.sceneManager.GetCurrentSceneSourcePath());
	m_panelsManager.GetPanelAs<Editor_SceneView>("Scene View").Focus();
}

bool Editor::Editor_EditorActions::IsCurrentSceneLoadedFromDisk() const
{
	return mContext.sceneManager.IsCurrentSceneLoadedFromDisk();
}

void Editor::Editor_EditorActions::SaveSceneChanges()
{
	if (IsCurrentSceneLoadedFromDisk())
	{
		SaveCurrentSceneTo(mContext.sceneManager.GetCurrentSceneSourcePath());
		//OVLOG_INFO("Current scene saved to: " + m_context.sceneManager.GetCurrentSceneSourcePath());
	}
	else
	{
		SaveAs();
	}
}

void Editor::Editor_EditorActions::SaveAs()
{
	Window::Window_SaveFileDialog dialog("New Scene");
	dialog.SetInitialDirectory(mContext.mProjectAssetsPath + "New Scene");
	dialog.DefineExtension("Overload Scene", ".ovscene");
	dialog.Show();

	if (dialog.HasSucceeded())
	{
		if (dialog.IsFileExisting())
		{
			Window::Window_MessageBox message("File already exists!", "The file \"" + dialog.GetSelectedFileName() + "\" already exists.\n\nUsing this file as the new home for your scene will erase any content stored in this file.\n\nAre you ok with that?", Window::Window_MessageBox::EMessageType::WARNING, Window::Window_MessageBox::EButtonLayout::YES_NO, true);
			switch (message.GetUserAction())
			{
			case Window::Window_MessageBox::EUserAction::YES: break;
			case Window::Window_MessageBox::EUserAction::NO: return;
			}
		}

		SaveCurrentSceneTo(dialog.GetSelectedFilePath());
		//OVLOG_INFO("Current scene saved to: " + dialog.GetSelectedFilePath());
	}
}

void Editor::Editor_EditorActions::RefreshScripts()
{
	//mContext.scriptInterpreter->RefreshAll();
	//m_panelsManager.GetPanelAs<Editor_Inspector>("Inspector").Refresh();
	//if (mContext.scriptInterpreter->IsOk())
	//{
	//	OVLOG_INFO("Scripts interpretation succeeded!");
	//}
}

std::optional<std::string> Editor::Editor_EditorActions::SelectBuildFolder()
{
	Window::Window_SaveFileDialog dialog("Build location");
	dialog.DefineExtension("Game Build", "..");
	dialog.Show();
	if (dialog.HasSucceeded())
	{
		std::string result = dialog.GetSelectedFilePath();
		result = std::string(result.data(), result.data() + result.size() - std::string("..").size()) + "\\";
		if (!std::filesystem::exists(result))
		{
			return result;
		}
		else
		{
			Window::Window_MessageBox message("Folder already exists!", "The folder \"" + result + "\" already exists.\n\nPlease select another location and try again", Window::Window_MessageBox::EMessageType::WARNING, Window::Window_MessageBox::EButtonLayout::OK, true);
			return {};
		}
	}
	else
	{
		return {};
	}
}

void Editor::Editor_EditorActions::Build(bool p_autoRun, bool p_tempFolder)
{
	std::string destinationFolder;

	if (p_tempFolder)
	{
		destinationFolder = std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\TempBuild\\";
		try
		{
			std::filesystem::remove_all(destinationFolder);
		}
		catch (std::filesystem::filesystem_error error)
		{
			Window::Window_MessageBox message("Temporary build failed", "The temporary folder is currently being used by another process", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK, true);
			return;
		}
	}
	else if (auto res = SelectBuildFolder(); res.has_value())
	{
		destinationFolder = res.value();
	}
	else
	{
		return;
	}

	BuildAtLocation(mContext.projectSettings.Get<bool>("dev_build") ? "Development" : "Shipping", destinationFolder, p_autoRun);
}

void Editor::Editor_EditorActions::BuildAtLocation(const std::string& p_configuration, const std::string p_buildPath, bool p_autoRun)
{
	std::string buildPath(p_buildPath);
	std::string executableName = mContext.projectSettings.Get<std::string>("executable_name") + ".exe";

	bool failed = false;

	//OVLOG_INFO("Preparing to build at location: \"" + buildPath + "\"");

	std::filesystem::remove_all(buildPath);

	if (std::filesystem::create_directory(buildPath))
	{
		//OVLOG_INFO("Build directory created");

		if (std::filesystem::create_directory(buildPath + "Data\\"))
		{
			//OVLOG_INFO("Data directory created");

			if (std::filesystem::create_directory(buildPath + "Data\\User\\"))
			{
				//OVLOG_INFO("Data\\User directory created");

				std::error_code err;

				std::filesystem::copy(mContext.mProjectFilePath, buildPath + "Data\\User\\Game.ini", err);

				if (!err)
				{
					//OVLOG_INFO("Data\\User\\Game.ini file generated");

					std::filesystem::copy(mContext.mProjectFilePath, buildPath + "Data\\User\\Assets\\", std::filesystem::copy_options::recursive, err);

					if (!std::filesystem::exists(buildPath + "Data\\User\\Assets\\" + (mContext.projectSettings.Get<std::string>("start_scene"))))
					{
						//OVLOG_ERROR("Failed to find Start Scene at expected path. Verify your Project Setings.");
						Window::Window_MessageBox message("Build Failure", "An error occured during the building of your game.\nCheck the console for more information", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK, true);
						std::filesystem::remove_all(buildPath);
						return;
					}

					if (!err)
					{
						//OVLOG_INFO("Data\\User\\Assets\\ directory copied");

						std::filesystem::copy(mContext.mProjectScriptsPath, buildPath + "Data\\User\\Scripts\\", std::filesystem::copy_options::recursive, err);

						if (!err)
						{
							//OVLOG_INFO("Data\\User\\Scripts\\ directory copied");

							std::filesystem::copy(mContext.mEngineAssetsPath, buildPath + "Data\\Engine\\", std::filesystem::copy_options::recursive, err);

							if (!err)
							{
								//OVLOG_INFO("Data\\Engine\\ directory copied");
							}
							else
							{
								//OVLOG_ERROR("Data\\Engine\\ directory failed to copy");
								failed = true;
							}
						}
						else
						{
							//OVLOG_ERROR("Data\\User\\Scripts\\ directory failed to copy");
							failed = true;
						}
					}
					else
					{
						//OVLOG_ERROR("Data\\User\\Assets\\ directory failed to copy");
						failed = true;
					}
				}
				else
				{
					//OVLOG_ERROR("Data\\User\\Game.ini file failed to generate");
					failed = true;
				}

				std::string builderFolder = "Builder\\" + p_configuration + "\\";

				if (std::filesystem::exists(builderFolder))
				{
					std::error_code err;

					std::filesystem::copy(builderFolder, buildPath, err);

					if (!err)
					{
						//OVLOG_INFO("Builder data (Dlls and executatble) copied");

						std::filesystem::rename(buildPath + "OvGame.exe", buildPath + executableName, err);

						if (!err)
						{
							//OVLOG_INFO("Game executable renamed to " + executableName);

							if (p_autoRun)
							{
								std::string exePath = buildPath + executableName;
								//OVLOG_INFO("Launching the game at location: \"" + exePath + "\"");
								if (std::filesystem::exists(exePath))
								{
									Tools::Tools_SystemCalls::OpenFile(exePath, buildPath);
								}
								else
								{
									//OVLOG_ERROR("Failed to start the game: Executable not found");
									failed = true;
								}
							}
						}
						else
						{
							//OVLOG_ERROR("Game executable failed to rename");
							failed = true;
						}
					}
					else
					{
						//OVLOG_ERROR("Builder data (Dlls and executatble) failed to copy");
						failed = true;
					}
				}
				else
				{
					const std::string buildConfiguration = p_configuration == "Development" ? "Debug" : "Release";
					//OVLOG_ERROR("Builder folder for \"" + p_configuration + "\" not found. Verify you have compiled Engine source code in '" + buildConfiguration + "' configuration.");
					failed = true;
				}
			}
		}
	}
	else
	{
		//OVLOG_ERROR("Build directory failed to create");
		failed = true;
	}

	if (failed)
	{
		std::filesystem::remove_all(buildPath);
		Window::Window_MessageBox message("Build Failure", "An error occured during the building of your game.\nCheck the console for more information", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK, true);
	}
}

void Editor::Editor_EditorActions::DelayAction(std::function<void()> p_action, uint32_t p_frames)
{
	m_delayedActions.emplace_back(p_frames + 1, p_action);
}

void Editor::Editor_EditorActions::ExecuteDelayedActions()
{
	std::for_each(m_delayedActions.begin(), m_delayedActions.end(), [](std::pair<uint32_t, std::function<void()>>& p_element)
		{
			--p_element.first;

			if (p_element.first == 0)
				p_element.second();
		});

	m_delayedActions.erase(std::remove_if(m_delayedActions.begin(), m_delayedActions.end(), [](std::pair<uint32_t, std::function<void()>>& p_element)
		{
			return p_element.first == 0;
		}), m_delayedActions.end());
}
