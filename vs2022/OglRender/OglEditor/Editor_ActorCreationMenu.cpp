#include <OglUI/UI_MenuList.h>
#include <OglUI/UI_MenuItem.h>

#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CDirectionalLight.h>
#include <OglCore/Core_CAmbientSphereLight.h>

#include "Editor_EditorActions.h"
#include "Editor_ActorCreationMenu.h"

void Editor::Editor_ActorCreationMenu::GenerateActorCreationMenu(UI::UI_MenuList& p_menuList, Core::Core_Actor* pParent, std::optional<std::function<void()>> p_onItemClicked)
{
}
