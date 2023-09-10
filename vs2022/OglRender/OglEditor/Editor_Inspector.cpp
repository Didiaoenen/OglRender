#include <map>

#include <OglUI/UI_Text.h>
#include <OglUI/UI_Image.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_Spacing.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_DDTarget.h>
#include <OglUI/UI_DragFloat.h>
#include <OglUI/UI_ColorEdit.h>
#include <OglUI/UI_InputFloat.h>
#include <OglUI/UI_DragMultipleFloats.h>

#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_TextureManager.h>
#include <OglCore/Core_ShaderManager.h>
#include <OglCore/Core_ModelManager.h>

#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CTransform.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CModelRenderer.h>
#include <OglCore/Core_CAmbientBoxLight.h>
#include <OglCore/Core_CDirectionalLight.h>
#include <OglCore/Core_CMaterialRenderer.h>

#include <OglWindow/Window_MessageBox.h>

#include "Editor_EditorActions.h"
#include "Editor_Inspector.h"

Editor::Editor_Inspector::Editor_Inspector(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings)
{
}

Editor::Editor_Inspector::~Editor_Inspector()
{
}

void Editor::Editor_Inspector::FocusActor(Core::Core_Actor& p_target)
{
}

void Editor::Editor_Inspector::UnFocus()
{
}

void Editor::Editor_Inspector::SoftUnFocus()
{
}

Core::Core_Actor* Editor::Editor_Inspector::GetTargetActor() const
{
	return nullptr;
}

void Editor::Editor_Inspector::CreateActorInspector(Core::Core_Actor& p_target)
{
}

void Editor::Editor_Inspector::DrawComponent(Core::Core_AComponent& p_component)
{
}

void Editor::Editor_Inspector::DrawBehaviour(Core::Core_Behaviour& p_behaviour)
{
}

void Editor::Editor_Inspector::Refresh()
{
}
