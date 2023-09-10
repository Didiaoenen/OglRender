
#include <OglCore/Core_GUIDrawer.h>
#include <OglCore/Core_MaterialLoader.h>

#include <OglUI/UI_Button.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_GroupCollapsable.h>

#include "Editor_EditorActions.h"
#include "Editor_ProjectSettings.h"

Editor::Editor_ProjectSettings::Editor_ProjectSettings(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	UI_PanelWindow(p_title, p_opened, p_windowSettings), m_projectFile(EDITOR_CONTEXT(projectSettings))
{
}
