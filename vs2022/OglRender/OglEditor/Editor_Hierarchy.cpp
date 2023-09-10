
#include <OglUI/UI_Button.h>
#include <OglUI/UI_CheckBox.h>
#include <OglUI/UI_DDSource.h>
#include <OglUI/UI_DDTarget.h>
#include <OglUI/UI_Separator.h>
#include <OglUI/UI_ContextualMenu.h>

#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_CAmbientBoxLight.h>
#include <OglCore/Core_CDirectionalLight.h>
#include <OglCore/Core_CAmbientSphereLight.h>

#include "Editor_ActorCreationMenu.h"
#include "Editor_EditorActions.h"
#include "Editor_Hierarchy.h"

Editor::Editor_Hierarchy::Editor_Hierarchy(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings)
{
}

void Editor::Editor_Hierarchy::Clear()
{
}

void Editor::Editor_Hierarchy::UnselectActorsWidgets()
{
}

void Editor::Editor_Hierarchy::SelectActorByInstance(Core::Core_Actor& p_actor)
{
}

void Editor::Editor_Hierarchy::SelectActorByWidget(UI::UI_TreeNode& p_widget)
{
}

void Editor::Editor_Hierarchy::AttachActorToParent(Core::Core_Actor& p_actor)
{
}

void Editor::Editor_Hierarchy::DetachFromParent(Core::Core_Actor& p_actor)
{
}

void Editor::Editor_Hierarchy::DeleteActorByInstance(Core::Core_Actor& p_actor)
{
}

void Editor::Editor_Hierarchy::AddActorByInstance(Core::Core_Actor& p_actor)
{
}
