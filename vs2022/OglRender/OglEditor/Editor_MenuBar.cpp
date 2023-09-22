#include <OglTools/Tools_SystemCalls.h>

#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CAmbientBoxLight.h>
#include <OglCore/Core_CDirectionalLight.h>
#include <OglCore/Core_CAmbientSphereLight.h>

#include <OglUI/UI_Button.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_SliderInt.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_SliderFloat.h>

#include "Editor_ActorCreationMenu.h"
#include "Editor_EditorSettings.h"
#include "Editor_EditorActions.h"
#include "Editor_SceneView.h"
#include "Editor_AssetView.h"
#include "Editor_MenuBar.h"

Editor::Editor_MenuBar::Editor_MenuBar()
{
	CreateFileMenu();
	CreateBuildMenu();
	CreateWindowMenu();
	CreateActorsMenu();
	CreateResourcesMenu();
	CreateSettingsMenu();
	CreateLayoutMenu();
	CreateHelpMenu();
}

void Editor::Editor_MenuBar::HandleShortcuts(float pDeltaTime)
{
	auto& inputManager = *EDITOR_CONTEXT(mInputManager);

	if (inputManager.GetKeyState(Window::EKey::KEY_LEFT_CONTROL) == Window::EKeyState::KEY_DOWN)
	{
		if (inputManager.IsKeyPressed(Window::EKey::KEY_N))
		{
			EDITOR_EXEC(LoadEmptyScene());
		}

		if (inputManager.IsKeyPressed(Window::EKey::KEY_S))
		{
			if (inputManager.GetKeyState(Window::EKey::KEY_LEFT_SHIFT) == Window::EKeyState::KEY_UP)
			{
				EDITOR_EXEC(SaveSceneChanges());
			}
			else
			{
				EDITOR_EXEC(SaveAs());
			}
		}
	}
}

void Editor::Editor_MenuBar::RegisterPanel(const std::string& pName, UI::UI_PanelWindow& pPanel)
{
	auto& menuItem = m_windowMenu->CreateWidget<UI::UI_MenuItem>(pName, "", true, true);
	menuItem.mValueChangedEvent += std::bind(&UI::UI_PanelWindow::SetOpened, &pPanel, std::placeholders::_1);

	m_panels.emplace(pName, std::make_pair(std::ref(pPanel), std::ref(menuItem)));
}

void Editor::Editor_MenuBar::CreateFileMenu()
{
	auto& fileMenu = CreateWidget<UI::UI_MenuList>("File");
	fileMenu.CreateWidget<UI::UI_MenuItem>("New Scene", "CTRL + N").mClickedEvent += EDITOR_BIND(LoadEmptyScene);
	fileMenu.CreateWidget<UI::UI_MenuItem>("Save Scene", "CTRL + S").mClickedEvent += EDITOR_BIND(SaveSceneChanges);
	fileMenu.CreateWidget<UI::UI_MenuItem>("Save Scene As...", "CTRL + SHIFT + S").mClickedEvent += EDITOR_BIND(SaveAs);
	fileMenu.CreateWidget<UI::UI_MenuItem>("Exit", "ALT + F4").mClickedEvent += [] { EDITOR_CONTEXT(mWindow)->SetShouldClose(true); };
}

void Editor::Editor_MenuBar::CreateBuildMenu()
{
	auto& buildMenu = CreateWidget<UI::UI_MenuList>("Build");
	buildMenu.CreateWidget<UI::UI_MenuItem>("Build game").mClickedEvent += EDITOR_BIND(Build, false, false);
	buildMenu.CreateWidget<UI::UI_MenuItem>("Build game and run").mClickedEvent += EDITOR_BIND(Build, true, false);
	buildMenu.CreateWidget<UI::UI_Separator>();
	buildMenu.CreateWidget<UI::UI_MenuItem>("Temporary build").mClickedEvent += EDITOR_BIND(Build, true, true);
}

void Editor::Editor_MenuBar::CreateWindowMenu()
{
	m_windowMenu = &CreateWidget<UI::UI_MenuList>("Window");
	m_windowMenu->CreateWidget<UI::UI_MenuItem>("Close all").mClickedEvent += std::bind(&Editor_MenuBar::OpenEveryWindows, this, false);
	m_windowMenu->CreateWidget<UI::UI_MenuItem>("Open all").mClickedEvent += std::bind(&Editor_MenuBar::OpenEveryWindows, this, true);
	m_windowMenu->CreateWidget<UI::UI_Separator>();

	m_windowMenu->mClickedEvent += std::bind(&Editor_MenuBar::UpdateToggleableItems, this);
}

void Editor::Editor_MenuBar::CreateActorsMenu()
{
	auto& actorsMenu = CreateWidget<UI::UI_MenuList>("Actors");
	Editor_ActorCreationMenu::GenerateActorCreationMenu(actorsMenu);
}

void Editor::Editor_MenuBar::CreateResourcesMenu()
{
	auto& resourcesMenu = CreateWidget<UI::UI_MenuList>("Resources");
	resourcesMenu.CreateWidget<UI::UI_MenuItem>("Compile shaders").mClickedEvent += EDITOR_BIND(CompileShaders);
	resourcesMenu.CreateWidget<UI::UI_MenuItem>("Save materials").mClickedEvent += EDITOR_BIND(SaveMaterials);
}

void Editor::Editor_MenuBar::CreateSettingsMenu()
{
	auto& settingsMenu = CreateWidget<UI::UI_MenuList>("Settings");
	settingsMenu.CreateWidget<UI::UI_MenuItem>("Spawn actors at origin", "", true, true).mValueChangedEvent += EDITOR_BIND(SetActorSpawnAtOrigin, std::placeholders::_1);
	settingsMenu.CreateWidget<UI::UI_MenuItem>("Vertical Synchronization", "", true, true).mValueChangedEvent += [this](bool p_value) { EDITOR_CONTEXT(mDevice)->SetVsync(p_value); };
	auto& cameraSpeedMenu = settingsMenu.CreateWidget<UI::UI_MenuList>("Camera Speed");
	cameraSpeedMenu.CreateWidget<UI::UI_SliderInt>(1, 50, 15, UI::ESliderOrientation::HORIZONTAL, "Scene View").mValueChangedEvent += EDITOR_BIND(SetSceneViewCameraSpeed, std::placeholders::_1);
	cameraSpeedMenu.CreateWidget<UI::UI_SliderInt>(1, 50, 15, UI::ESliderOrientation::HORIZONTAL, "Asset View").mValueChangedEvent += EDITOR_BIND(SetAssetViewCameraSpeed, std::placeholders::_1);
	auto& cameraPositionMenu = settingsMenu.CreateWidget<UI::UI_MenuList>("Reset Camera");
	cameraPositionMenu.CreateWidget<UI::UI_MenuItem>("Scene View").mClickedEvent += EDITOR_BIND(ResetSceneViewCameraPosition);
	cameraPositionMenu.CreateWidget<UI::UI_MenuItem>("Asset View").mClickedEvent += EDITOR_BIND(ResetAssetViewCameraPosition);

	auto& viewColors = settingsMenu.CreateWidget<UI::UI_MenuList>("View Colors");
	auto& sceneViewBackground = viewColors.CreateWidget<UI::UI_MenuList>("Scene View Background");
	auto& sceneViewBackgroundPicker = sceneViewBackground.CreateWidget<UI::UI_ColorEdit>(false, UI::Color{ 0.098f, 0.098f, 0.098f, 1.f });
	sceneViewBackgroundPicker.mColorChangedEvent += [this](const auto& color)
		{
			EDITOR_PANEL(Editor_SceneView, "Scene View").GetCamera().SetClearColor({ color.r, color.g, color.b });
		};
	sceneViewBackground.CreateWidget<UI::UI_MenuItem>("Reset").mClickedEvent += [this, &sceneViewBackgroundPicker]
		{
			EDITOR_PANEL(Editor_SceneView, "Scene View").GetCamera().SetClearColor({ 0.098f, 0.098f, 0.098f });
			sceneViewBackgroundPicker.mColor = { 0.098f, 0.098f, 0.098f, 1.f };
		};

	auto& sceneViewGrid = viewColors.CreateWidget<UI::UI_MenuList>("Scene View Grid");
	auto& sceneViewGridPicker = sceneViewGrid.CreateWidget<UI::UI_ColorEdit>(false, UI::Color(0.176f, 0.176f, 0.176f, 1.f));
	sceneViewGridPicker.mColorChangedEvent += [this](const auto& color)
		{
			EDITOR_PANEL(Editor_SceneView, "Scene View").SetGridColor({ color.r, color.g, color.b });
		};
	sceneViewGrid.CreateWidget<UI::UI_MenuItem>("Reset").mClickedEvent += [this, &sceneViewGridPicker]
		{
			EDITOR_PANEL(Editor_SceneView, "Scene View").SetGridColor(glm::vec3(0.176f, 0.176f, 0.176f));
			sceneViewGridPicker.mColor = UI::Color(0.176f, 0.176f, 0.176f, 1.f);
		};

	auto& assetViewBackground = viewColors.CreateWidget<UI::UI_MenuList>("Asset View Background");
	auto& assetViewBackgroundPicker = assetViewBackground.CreateWidget<UI::UI_ColorEdit>(false, UI::Color{ 0.098f, 0.098f, 0.098f, 1.f });
	assetViewBackgroundPicker.mColorChangedEvent += [this](const auto& color)
		{
			EDITOR_PANEL(Editor_AssetView, "Asset View").GetCamera().SetClearColor({ color.r, color.g, color.b });
		};
	assetViewBackground.CreateWidget<UI::UI_MenuItem>("Reset").mClickedEvent += [this, &assetViewBackgroundPicker]
		{
			EDITOR_PANEL(Editor_AssetView, "Asset View").GetCamera().SetClearColor({ 0.098f, 0.098f, 0.098f });
			assetViewBackgroundPicker.mColor = { 0.098f, 0.098f, 0.098f, 1.f };
		};

	auto& assetViewGrid = viewColors.CreateWidget<UI::UI_MenuList>("Asset View Grid");
	auto& assetViewGridPicker = assetViewGrid.CreateWidget<UI::UI_ColorEdit>(false, UI::Color(0.176f, 0.176f, 0.176f, 1.f));
	assetViewGridPicker.mColorChangedEvent += [this](const auto& color)
		{
			EDITOR_PANEL(Editor_AssetView, "Asset View").SetGridColor({ color.r, color.g, color.b });
		};
	assetViewGrid.CreateWidget<UI::UI_MenuItem>("Reset").mClickedEvent += [this, &assetViewGridPicker]
		{
			EDITOR_PANEL(Editor_AssetView, "Asset View").SetGridColor(glm::vec3(0.176f, 0.176f, 0.176f));
			assetViewGridPicker.mColor = UI::Color(0.176f, 0.176f, 0.176f, 1.f);
		};

	auto& sceneViewBillboardScaleMenu = settingsMenu.CreateWidget<UI::UI_MenuList>("3D Icons Scales");
	auto& lightBillboardScaleSlider = sceneViewBillboardScaleMenu.CreateWidget<UI::UI_SliderInt>(0, 100, static_cast<int>(Editor_EditorSettings::LightBillboardScale * 100.0f), UI::ESliderOrientation::HORIZONTAL, "Lights");
	lightBillboardScaleSlider.mValueChangedEvent += [this](int p_value) { Editor_EditorSettings::LightBillboardScale = p_value / 100.0f; };
	lightBillboardScaleSlider.mFormat = "%d %%";

	auto& snappingMenu = settingsMenu.CreateWidget<UI::UI_MenuList>("Snapping");
	snappingMenu.CreateWidget<UI::UI_DragFloat>(0.001f, 999999.0f, Editor_EditorSettings::TranslationSnapUnit, 0.05f, "Translation Unit").mValueChangedEvent += [this](float p_value) { Editor_EditorSettings::TranslationSnapUnit = p_value; };
	snappingMenu.CreateWidget<UI::UI_DragFloat>(0.001f, 999999.0f, Editor_EditorSettings::RotationSnapUnit, 1.0f, "Rotation Unit").mValueChangedEvent += [this](float p_value) { Editor_EditorSettings::RotationSnapUnit = p_value; };
	snappingMenu.CreateWidget<UI::UI_DragFloat>(0.001f, 999999.0f, Editor_EditorSettings::ScalingSnapUnit, 0.05f, "Scaling Unit").mValueChangedEvent += [this](float p_value) { Editor_EditorSettings::ScalingSnapUnit = p_value; };

	auto& debuggingMenu = settingsMenu.CreateWidget<UI::UI_MenuList>("Debugging");
	debuggingMenu.CreateWidget<UI::UI_MenuItem>("Show geometry bounds", "", true, Editor_EditorSettings::ShowGeometryBounds).mValueChangedEvent += [this](bool p_value) { Editor_EditorSettings::ShowGeometryBounds = p_value; };
	debuggingMenu.CreateWidget<UI::UI_MenuItem>("Show lights bounds", "", true, Editor_EditorSettings::ShowLightBounds).mValueChangedEvent += [this](bool p_value) { Editor_EditorSettings::ShowLightBounds = p_value; };
	auto& subMenu = debuggingMenu.CreateWidget<UI::UI_MenuList>("Frustum culling visualizer...");
	subMenu.CreateWidget<UI::UI_MenuItem>("For geometry", "", true, Editor_EditorSettings::ShowGeometryFrustumCullingInSceneView).mValueChangedEvent += [this](bool p_value) { Editor_EditorSettings::ShowGeometryFrustumCullingInSceneView = p_value; };
	subMenu.CreateWidget<UI::UI_MenuItem>("For lights", "", true, Editor_EditorSettings::ShowLightFrustumCullingInSceneView).mValueChangedEvent += [this](bool p_value) { Editor_EditorSettings::ShowLightFrustumCullingInSceneView = p_value; };
}

void Editor::Editor_MenuBar::CreateLayoutMenu()
{
	auto& layoutMenu = CreateWidget<UI::UI_MenuList>("Layout");
	layoutMenu.CreateWidget<UI::UI_MenuItem>("Reset").mClickedEvent += EDITOR_BIND(ResetLayout);
}

void Editor::Editor_MenuBar::CreateHelpMenu()
{
	auto& helpMenu = CreateWidget<UI::UI_MenuList>("Help");
	helpMenu.CreateWidget<UI::UI_MenuItem>("GitHub").mClickedEvent += [] {Tools::Tools_SystemCalls::OpenURL("https://github.com/adriengivry/Overload"); };
	helpMenu.CreateWidget<UI::UI_MenuItem>("Tutorials").mClickedEvent += [] {Tools::Tools_SystemCalls::OpenURL("https://github.com/adriengivry/Overload/wiki/Tutorials"); };
	helpMenu.CreateWidget<UI::UI_MenuItem>("Scripting API").mClickedEvent += [] {Tools::Tools_SystemCalls::OpenURL("https://github.com/adriengivry/Overload/wiki/Scripting-API"); };
	helpMenu.CreateWidget<UI::UI_Separator>();
	helpMenu.CreateWidget<UI::UI_MenuItem>("Bug Report").mClickedEvent += [] {Tools::Tools_SystemCalls::OpenURL("https://github.com/adriengivry/Overload/issues/new?assignees=&labels=Bug&template=bug_report.md&title="); };
	helpMenu.CreateWidget<UI::UI_MenuItem>("Feature Request").mClickedEvent += [] {Tools::Tools_SystemCalls::OpenURL("https://github.com/adriengivry/Overload/issues/new?assignees=&labels=Feature&template=feature_request.md&title="); };
	helpMenu.CreateWidget<UI::UI_Separator>();
	helpMenu.CreateWidget<UI::UI_Text>("Version: 1.3.0");
}

void Editor::Editor_MenuBar::UpdateToggleableItems()
{
	for (auto& [name, panel] : m_panels)
	{
		panel.second.get().mChecked = panel.first.get().IsOpened();
	}
}

void Editor::Editor_MenuBar::OpenEveryWindows(bool pState)
{
	for (auto& [name, panel] : m_panels)
	{
		panel.first.get().SetOpened(pState);
	}
}
