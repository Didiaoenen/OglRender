
#include <OglCore/Core_GUIDrawer.h>
#include <OglCore/Core_MaterialLoader.h>

#include <OglUI/UI_Button.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_TextColored.h>
#include <OglUI/UI_ButtonSmall.h>
#include <OglUI/UI_GroupCollapsable.h>

#include "Editor_AssetView.h"
#include "Editor_EditorActions.h"
#include "Editor_MaterialEditor.h"

void DrawHybridVec3(UI::UI_WidgetContainer& p_root, const std::string& p_name, glm::vec3& pData, float p_step, float p_min, float p_max)
{
	Core::Core_GUIDrawer::CreateTitle(p_root, p_name);

	auto& rightSide = p_root.CreateWidget<UI::UI_Group>();

	auto& xyzWidget = rightSide.CreateWidget<UI::UI_DragMultipleScalars<float, 3>>(Core::Core_GUIDrawer::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", Core::Core_GUIDrawer::GetFormat<float>());
	auto& xyzDispatcher = xyzWidget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 3>>>();
	xyzDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(pData));
	xyzWidget.mLineBreak = false;

	auto& rgbWidget = rightSide.CreateWidget<UI::UI_ColorEdit>(false, UI::Color{ pData.x, pData.y, pData.z, 1.f });
	auto& rgbDispatcher = rgbWidget.AddPlugin<UI::UI_DataDispatcher<UI::Color>>();
	rgbDispatcher.RegisterReference(reinterpret_cast<UI::Color&>(pData));
	rgbWidget.mEnabled = false;
	rgbWidget.mLineBreak = false;

	auto& xyzButton = rightSide.CreateWidget<UI::UI_Button>("XYZ");
	xyzButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };
	xyzButton.mLineBreak = false;

	auto& rgbButton = rightSide.CreateWidget<UI::UI_Button>("RGB");
	rgbButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };

	xyzButton.mClickedEvent += [&]
		{
			xyzWidget.mEnabled = true;
			rgbWidget.mEnabled = false;
		};

	rgbButton.mClickedEvent += [&]
		{
			xyzWidget.mEnabled = false;
			rgbWidget.mEnabled = true;
		};
}

void DrawHybridVec4(UI::UI_WidgetContainer& p_root, const std::string& p_name, glm::vec4& pData, float p_step, float p_min, float p_max)
{
	Core::Core_GUIDrawer::CreateTitle(p_root, p_name);

	auto& rightSide = p_root.CreateWidget<UI::UI_Group>();

	auto& xyzWidget = rightSide.CreateWidget<UI::UI_DragMultipleScalars<float, 4>>(Core::Core_GUIDrawer::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", Core::Core_GUIDrawer::GetFormat<float>());
	auto& xyzDispatcher = xyzWidget.AddPlugin<UI::UI_DataDispatcher<std::array<float, 4>>>();
	xyzDispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(pData));
	xyzWidget.mLineBreak = false;

	auto& rgbaWidget = rightSide.CreateWidget<UI::UI_ColorEdit>(true, UI::Color{ pData.x, pData.y, pData.z, pData.w });
	auto& rgbaDispatcher = rgbaWidget.AddPlugin<UI::UI_DataDispatcher<UI::Color>>();
	rgbaDispatcher.RegisterReference(reinterpret_cast<UI::Color&>(pData));
	rgbaWidget.mEnabled = false;
	rgbaWidget.mLineBreak = false;

	auto& xyzwButton = rightSide.CreateWidget<UI::UI_Button>("XYZW");
	xyzwButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };
	xyzwButton.mLineBreak = false;

	auto& rgbaButton = rightSide.CreateWidget<UI::UI_Button>("RGBA");
	rgbaButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };

	xyzwButton.mClickedEvent += [&]
		{
			xyzWidget.mEnabled = true;
			rgbaWidget.mEnabled = false;
		};

	rgbaButton.mClickedEvent += [&]
		{
			xyzWidget.mEnabled = false;
			rgbaWidget.mEnabled = true;
		};
}


Editor::Editor_MaterialEditor::Editor_MaterialEditor(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) :
	UI::UI_PanelWindow(pTitle, pOpened, pWindowSettings)
{
	CreateHeaderButtons();
	CreateWidget<UI::UI_Separator>();
	CreateMaterialSelector();
	m_settings = &CreateWidget<UI::UI_Group>();
	CreateShaderSelector();
	CreateMaterialSettings();
	CreateShaderSettings();

	m_settings->mEnabled = false;
	m_shaderSettings->mEnabled = false;

	m_materialDroppedEvent += std::bind(&Editor_MaterialEditor::OnMaterialDropped, this);
	m_shaderDroppedEvent += std::bind(&Editor_MaterialEditor::OnShaderDropped, this);
}

void Editor::Editor_MaterialEditor::Refresh()
{
	if (m_target)
	{
		SetTarget(*m_target);
	}
}

void Editor::Editor_MaterialEditor::SetTarget(Core::Core_Material& p_newTarget)
{
	m_target = &p_newTarget;
	m_targetMaterialText->mContent = m_target->mPath;
	OnMaterialDropped();
}

Core::Core_Material* Editor::Editor_MaterialEditor::GetTarget() const
{
	return m_target;
}

void Editor::Editor_MaterialEditor::RemoveTarget()
{
	m_target = nullptr;
	m_targetMaterialText->mContent = "Empty";
	OnMaterialDropped();
}

void Editor::Editor_MaterialEditor::Preview()
{
	auto& assetView = EDITOR_PANEL(Editor_AssetView, "Asset View");

	if (m_target)
		assetView.SetResource(m_target);

	assetView.Open();
}

void Editor::Editor_MaterialEditor::Reset()
{
	if (m_target && mShader)
	{
		m_target->SetShader(nullptr);
		OnShaderDropped();
	}
}

void Editor::Editor_MaterialEditor::OnMaterialDropped()
{
	m_settings->mEnabled = m_target;

	if (m_settings->mEnabled)
	{
		GenerateMaterialSettingsContent();
		m_shaderText->mContent = m_target->GetShader() ? m_target->GetShader()->mPath : "Empty";
		mShader = m_target->GetShader();
	}
	else
	{
		m_materialSettingsColumns->RemoveAllWidgets();
	}

	m_shaderSettings->mEnabled = false;
	m_shaderSettingsColumns->RemoveAllWidgets();

	if (m_target && m_target->GetShader())
	{
		OnShaderDropped();
	}
}

void Editor::Editor_MaterialEditor::OnShaderDropped()
{
	m_shaderSettings->mEnabled = mShader;

	if (mShader != m_target->GetShader())
	{
		m_target->SetShader(mShader);
	}

	if (m_shaderSettings->mEnabled)
	{
		GenerateShaderSettingsContent();
	}
	else
	{
		m_shaderSettingsColumns->RemoveAllWidgets();
	}
}

void Editor::Editor_MaterialEditor::CreateHeaderButtons()
{
	auto& saveButton = CreateWidget<UI::UI_Button>("Save to file");
	saveButton.mIdleBackgroundColor = { 0.0f, 0.5f, 0.0f, 1.f };
	saveButton.mClickedEvent += [this]
		{
			if (m_target)
			{
				Core::Core_MaterialLoader::Save(*m_target, EDITOR_EXEC(GetRealPath(m_target->mPath)));
			}
		};

	saveButton.mLineBreak = false;

	auto& reloadButton = CreateWidget<UI::UI_Button>("Reload from file");
	reloadButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };
	reloadButton.mClickedEvent += [this]
		{
			if (m_target)
				Core::Core_MaterialLoader::Reload(*m_target, EDITOR_EXEC(GetRealPath(m_target->mPath)));

			OnMaterialDropped();
		};

	reloadButton.mLineBreak = false;

	auto& previewButton = CreateWidget<UI::UI_Button>("Preview");
	previewButton.mIdleBackgroundColor = { 0.7f, 0.5f, 0.0f, 1.f };
	previewButton.mClickedEvent += std::bind(&Editor_MaterialEditor::Preview, this);
	previewButton.mLineBreak = false;

	auto& resetButton = CreateWidget<UI::UI_Button>("Reset to default");
	resetButton.mIdleBackgroundColor = { 0.5f, 0.0f, 0.0f, 1.f };
	resetButton.mClickedEvent += std::bind(&Editor_MaterialEditor::Reset, this);
}

void Editor::Editor_MaterialEditor::CreateMaterialSelector()
{
	auto& columns = CreateWidget<UI::UI_Columns<2>>();
	columns.mWidths[0] = 150;
	m_targetMaterialText = &Core::Core_GUIDrawer::DrawMaterial(columns, "Material", m_target, &m_materialDroppedEvent);
}

void Editor::Editor_MaterialEditor::CreateShaderSelector()
{
	auto& columns = m_settings->CreateWidget<UI::UI_Columns<2>>();
	columns.mWidths[0] = 150;
	m_shaderText = &Core::Core_GUIDrawer::DrawShader(columns, "Shader", mShader, &m_shaderDroppedEvent);
}

void Editor::Editor_MaterialEditor::CreateMaterialSettings()
{
	m_materialSettings = &m_settings->CreateWidget<UI::UI_GroupCollapsable>("Material Settings");
	m_materialSettingsColumns = &m_materialSettings->CreateWidget<UI::UI_Columns<2>>();
	m_materialSettingsColumns->mWidths[0] = 150;
}

void Editor::Editor_MaterialEditor::CreateShaderSettings()
{
	m_shaderSettings = &m_settings->CreateWidget<UI::UI_GroupCollapsable>("Shader Settings");
	m_shaderSettingsColumns = &m_shaderSettings->CreateWidget<UI::UI_Columns<2>>();
	m_shaderSettingsColumns->mWidths[0] = 150;
}

std::string UniformFormat(const std::string& p_string)
{
	std::string result;
	std::string formattedInput = p_string;

	if (formattedInput.rfind("u_", 0) == 0 || formattedInput.rfind("U_", 0) == 0)
	{
		formattedInput = formattedInput.substr(2);
	}

	std::string capsChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	bool first = true;
	bool capsNext = false;

	for (char c : formattedInput)
	{
		if (first || capsNext)
		{
			c = toupper(c);
			first = false;
			capsNext = false;
		}

		if (c == '_')
		{
			c = ' ';
			capsNext = true;
		}

		if (std::find(capsChars.begin(), capsChars.end(), c) != capsChars.end())
		{
			result.push_back(' ');
		}

		result.push_back(c);
	}

	return result;
}

void Editor::Editor_MaterialEditor::GenerateShaderSettingsContent()
{
	m_shaderSettingsColumns->RemoveAllWidgets();

	std::multimap<int, std::pair<std::string, std::any*>> sortedUniformsData;

	for (auto& [name, value] : m_target->GetUniformsData())
	{
		int orderID = 0;

		auto uniformData = m_target->GetShader()->GetUniformInfo(name);

		if (uniformData)
		{
			switch (uniformData->type)
			{
				case Render::EUniformType::UNIFORM_SAMPLER_2D:	orderID = 0; break;
				case Render::EUniformType::UNIFORM_FLOAT_VEC4:	orderID = 1; break;
				case Render::EUniformType::UNIFORM_FLOAT_VEC3:	orderID = 2; break;
				case Render::EUniformType::UNIFORM_FLOAT_VEC2:	orderID = 3; break;
				case Render::EUniformType::UNIFORM_FLOAT:		orderID = 4; break;
				case Render::EUniformType::UNIFORM_INT:			orderID = 5; break;
				case Render::EUniformType::UNIFORM_BOOL:			orderID = 6; break;
			}

			sortedUniformsData.emplace(orderID, std::pair<std::string, std::any*>{ name, & value });
		}
	}

	for (auto& [order, info] : sortedUniformsData)
	{
		auto uniformData = m_target->GetShader()->GetUniformInfo(info.first);

		if (uniformData)
		{
			switch (uniformData->type)
			{
				case Render::EUniformType::UNIFORM_BOOL:			Core::Core_GUIDrawer::DrawBoolean(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<bool&>(*info.second));																	break;
				case Render::EUniformType::UNIFORM_INT:			Core::Core_GUIDrawer::DrawScalar<int>(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<int&>(*info.second));																break;
				case Render::EUniformType::UNIFORM_FLOAT:		Core::Core_GUIDrawer::DrawScalar<float>(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<float&>(*info.second), 0.01f, Core::Core_GUIDrawer::_MIN_FLOAT, Core::Core_GUIDrawer::_MAX_FLOAT);		break;
				case Render::EUniformType::UNIFORM_FLOAT_VEC2:	Core::Core_GUIDrawer::DrawVec2(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<glm::vec2&>(*info.second), 0.01f, Core::Core_GUIDrawer::_MIN_FLOAT, Core::Core_GUIDrawer::_MAX_FLOAT);	break;
				case Render::EUniformType::UNIFORM_FLOAT_VEC3:	DrawHybridVec3(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<glm::vec3&>(*info.second), 0.01f, Core::Core_GUIDrawer::_MIN_FLOAT, Core::Core_GUIDrawer::_MAX_FLOAT);			break;
				case Render::EUniformType::UNIFORM_FLOAT_VEC4:	DrawHybridVec4(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<glm::vec4&>(*info.second), 0.01f, Core::Core_GUIDrawer::_MIN_FLOAT, Core::Core_GUIDrawer::_MAX_FLOAT);			break;
				case Render::EUniformType::UNIFORM_SAMPLER_2D:	Core::Core_GUIDrawer::DrawTexture(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<Render::Render_Texture*&>(*info.second));																break;
			}
		}
	}
}

void Editor::Editor_MaterialEditor::GenerateMaterialSettingsContent()
{
	m_materialSettingsColumns->RemoveAllWidgets();

	Core::Core_GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Blendable", std::bind(&Core::Core_Material::IsBlendable, m_target), std::bind(&Core::Core_Material::SetBlendable, m_target, std::placeholders::_1));
	Core::Core_GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Back-face Culling", std::bind(&Core::Core_Material::HasBackfaceCulling, m_target), std::bind(&Core::Core_Material::SetBackfaceCulling, m_target, std::placeholders::_1));
	Core::Core_GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Front-face Culling", std::bind(&Core::Core_Material::HasFrontfaceCulling, m_target), std::bind(&Core::Core_Material::SetFrontfaceCulling, m_target, std::placeholders::_1));
	Core::Core_GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Depth Test", std::bind(&Core::Core_Material::HasDepthTest, m_target), std::bind(&Core::Core_Material::SetDepthTest, m_target, std::placeholders::_1));
	Core::Core_GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Depth Writing", std::bind(&Core::Core_Material::HasDepthWriting, m_target), std::bind(&Core::Core_Material::SetDepthWriting, m_target, std::placeholders::_1));
	Core::Core_GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Color Writing", std::bind(&Core::Core_Material::HasColorWriting, m_target), std::bind(&Core::Core_Material::SetColorWriting, m_target, std::placeholders::_1));
	Core::Core_GUIDrawer::DrawScalar<int>(*m_materialSettingsColumns, "GPU Instances", std::bind(&Core::Core_Material::GetGPUInstances, m_target), std::bind(&Core::Core_Material::SetGPUInstances, m_target, std::placeholders::_1), 1.0f, 0, 100000);
}
