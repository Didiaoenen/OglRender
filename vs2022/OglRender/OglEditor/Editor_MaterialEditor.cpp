
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

Editor::Editor_MaterialEditor::Editor_MaterialEditor(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings)
{
}

void Editor::Editor_MaterialEditor::Refresh()
{
}

void Editor::Editor_MaterialEditor::SetTarget(Core::Core_Material& p_newTarget)
{
}

Core::Core_Material* Editor::Editor_MaterialEditor::GetTarget() const
{
	return nullptr;
}

void Editor::Editor_MaterialEditor::RemoveTarget()
{
}

void Editor::Editor_MaterialEditor::Preview()
{
}

void Editor::Editor_MaterialEditor::Reset()
{
}

void Editor::Editor_MaterialEditor::OnMaterialDropped()
{
}

void Editor::Editor_MaterialEditor::OnShaderDropped()
{
}

void Editor::Editor_MaterialEditor::CreateHeaderButtons()
{
}

void Editor::Editor_MaterialEditor::CreateMaterialSelector()
{
}

void Editor::Editor_MaterialEditor::CreateShaderSelector()
{
}

void Editor::Editor_MaterialEditor::CreateMaterialSettings()
{
}

void Editor::Editor_MaterialEditor::CreateShaderSettings()
{
}

void Editor::Editor_MaterialEditor::GenerateShaderSettingsContent()
{
}

void Editor::Editor_MaterialEditor::GenerateMaterialSettingsContent()
{
}
