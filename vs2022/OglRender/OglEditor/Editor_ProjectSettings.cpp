
#include <OglCore/Core_GUIDrawer.h>
#include <OglCore/Core_MaterialLoader.h>

#include <OglUI/UI_Button.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_GroupCollapsable.h>

#include "Editor_EditorActions.h"
#include "Editor_ProjectSettings.h"

Editor::Editor_ProjectSettings::Editor_ProjectSettings(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) :
	UI_PanelWindow(pTitle, pOpened, pWindowSettings), m_projectFile(EDITOR_CONTEXT(projectSettings))
{
	auto& saveButton = CreateWidget<UI::UI_Button>("Apply");
	saveButton.mIdleBackgroundColor = { 0.0f, 0.5f, 0.0f, 1.f };
	saveButton.mClickedEvent += [this]
		{
			EDITOR_CONTEXT(ApplyProjectSettings());
			m_projectFile.Rewrite();
		};

	saveButton.mLineBreak = false;

	auto& resetButton = CreateWidget<UI::UI_Button>("Reset");
	resetButton.mIdleBackgroundColor = { 0.5f, 0.0f, 0.0f, 1.f };
	resetButton.mClickedEvent += [this]
		{
			EDITOR_CONTEXT(ResetProjectSettings());
		};

	CreateWidget<UI::UI_Separator>();

	{
		auto& root = CreateWidget<UI::UI_GroupCollapsable>("Physics");
		auto& columns = root.CreateWidget<UI::UI_Columns<2>>();
		columns.mWidths[0] = 125;

		Core::Core_GUIDrawer::DrawScalar<float>(columns, "Gravity", GenerateGatherer<float>("gravity"), GenerateProvider<float>("gravity"), 0.1f, Core::Core_GUIDrawer::_MIN_FLOAT, Core::Core_GUIDrawer::_MAX_FLOAT);
	}

	{
		auto& generationRoot = CreateWidget<UI::UI_GroupCollapsable>("Build");
		auto& columns = generationRoot.CreateWidget<UI::UI_Columns<2>>();
		columns.mWidths[0] = 125;

		Core::Core_GUIDrawer::DrawBoolean(columns, "Development build", GenerateGatherer<bool>("dev_build"), GenerateProvider<bool>("dev_build"));
	}

	{
		auto& windowingRoot = CreateWidget<UI::UI_GroupCollapsable>("Windowing");
		auto& columns = windowingRoot.CreateWidget<UI::UI_Columns<2>>();
		columns.mWidths[0] = 125;

		Core::Core_GUIDrawer::DrawScalar<int>(columns, "Resolution X", GenerateGatherer<int>("x_resolution"), GenerateProvider<int>("x_resolution"), 1, 0, 10000);
		Core::Core_GUIDrawer::DrawScalar<int>(columns, "Resolution Y", GenerateGatherer<int>("y_resolution"), GenerateProvider<int>("y_resolution"), 1, 0, 10000);
		Core::Core_GUIDrawer::DrawBoolean(columns, "Fullscreen", GenerateGatherer<bool>("fullscreen"), GenerateProvider<bool>("fullscreen"));
		Core::Core_GUIDrawer::DrawString(columns, "Executable name", GenerateGatherer<std::string>("executable_name"), GenerateProvider<std::string>("executable_name"));
	}

	{
		auto& renderingRoot = CreateWidget<UI::UI_GroupCollapsable>("Rendering");
		auto& columns = renderingRoot.CreateWidget<UI::UI_Columns<2>>();
		columns.mWidths[0] = 125;

		Core::Core_GUIDrawer::DrawBoolean(columns, "Vertical Sync.", GenerateGatherer<bool>("vsync"), GenerateProvider<bool>("vsync"));
		Core::Core_GUIDrawer::DrawBoolean(columns, "Multi-sampling", GenerateGatherer<bool>("multisampling"), GenerateProvider<bool>("multisampling"));
		Core::Core_GUIDrawer::DrawScalar<int>(columns, "Samples", GenerateGatherer<int>("samples"), GenerateProvider<int>("samples"), 1, 2, 16);
		Core::Core_GUIDrawer::DrawScalar<int>(columns, "OpenGL Major", GenerateGatherer<int>("opengl_major"), GenerateProvider<int>("opengl_major"), 1, 3, 4);
		Core::Core_GUIDrawer::DrawScalar<int>(columns, "OpenGL Minor", GenerateGatherer<int>("opengl_minor"), GenerateProvider<int>("opengl_minor"), 1, 0, 6);
	}

	{
		auto& gameRoot = CreateWidget<UI::UI_GroupCollapsable>("Scene Management");
		auto& columns = gameRoot.CreateWidget<UI::UI_Columns<2>>();
		columns.mWidths[0] = 125;

		Core::Core_GUIDrawer::DrawDDString(columns, "Start scene", GenerateGatherer<std::string>("start_scene"), GenerateProvider<std::string>("start_scene"), "File");
	}
}
