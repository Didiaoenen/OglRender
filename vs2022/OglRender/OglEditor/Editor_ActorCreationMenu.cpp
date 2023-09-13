#include <OglUI/UI_MenuList.h>
#include <OglUI/UI_MenuItem.h>

#include <OglCore/Core_CCamera.h>
#include <OglCore/Core_CSpotLight.h>
#include <OglCore/Core_CPointLight.h>
#include <OglCore/Core_CDirectionalLight.h>
#include <OglCore/Core_CAmbientSphereLight.h>

#include "Editor_EditorActions.h"
#include "Editor_ActorCreationMenu.h"

std::function<void()> Combine(std::function<void()> p_a, std::optional<std::function<void()>> p_b)
{
    if (p_b.has_value())
    {
        return [=]()
            {
                p_a();
                p_b.value()();
            };
    }

    return p_a;
}

template<class T>
std::function<void()> ActorWithComponentCreationHandler(Core::Core_Actor* pParent, std::optional<std::function<void()>> p_onItemClicked)
{
    return Combine(EDITOR_BIND(CreateMonoComponentActor<T>, true, pParent), p_onItemClicked);
}

std::function<void()> ActorWithModelComponentCreationHandler(Core::Core_Actor* pParent, const std::string& p_modelName, std::optional<std::function<void()>> p_onItemClicked)
{
    return Combine(EDITOR_BIND(CreateActorWithModel, ":Models\\" + p_modelName + ".fbx", true, pParent, p_modelName), p_onItemClicked);
}

void Editor::Editor_ActorCreationMenu::GenerateActorCreationMenu(UI::UI_MenuList& p_menuList, Core::Core_Actor* pParent, std::optional<std::function<void()>> p_onItemClicked)
{
    p_menuList.CreateWidget<UI::UI_MenuItem>("Create Empty").mClickedEvent += Combine(EDITOR_BIND(Editor_EditorActions::CreateEmptyActor, true, pParent, ""), p_onItemClicked);

    auto& primitives = p_menuList.CreateWidget<UI::UI_MenuList>("Primitives");
    auto& physicals = p_menuList.CreateWidget<UI::UI_MenuList>("Physicals");
    auto& lights = p_menuList.CreateWidget<UI::UI_MenuList>("Lights");
    auto& audio = p_menuList.CreateWidget<UI::UI_MenuList>("Audio");
    auto& others = p_menuList.CreateWidget<UI::UI_MenuList>("Others");

    primitives.CreateWidget<UI::UI_MenuItem>("Cube").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Cube", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Sphere").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Sphere", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Cone").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Cone", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Cylinder").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Cylinder", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Plane").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Plane", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Gear").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Gear", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Helix").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Helix", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Pipe").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Pipe", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Pyramid").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Pyramid", p_onItemClicked);
    primitives.CreateWidget<UI::UI_MenuItem>("Torus").mClickedEvent += ActorWithModelComponentCreationHandler(pParent, "Torus", p_onItemClicked);
    lights.CreateWidget<UI::UI_MenuItem>("Point").mClickedEvent += ActorWithComponentCreationHandler<Core::Core_CPointLight>(pParent, p_onItemClicked);
    lights.CreateWidget<UI::UI_MenuItem>("Directional").mClickedEvent += ActorWithComponentCreationHandler<Core::Core_CDirectionalLight>(pParent, p_onItemClicked);
    lights.CreateWidget<UI::UI_MenuItem>("Spot").mClickedEvent += ActorWithComponentCreationHandler<Core::Core_CSpotLight>(pParent, p_onItemClicked);
    lights.CreateWidget<UI::UI_MenuItem>("Ambient Box").mClickedEvent += ActorWithComponentCreationHandler<Core::Core_CAmbientBoxLight>(pParent, p_onItemClicked);
    lights.CreateWidget<UI::UI_MenuItem>("Ambient Sphere").mClickedEvent += ActorWithComponentCreationHandler<Core::Core_CAmbientSphereLight>(pParent, p_onItemClicked);
    others.CreateWidget<UI::UI_MenuItem>("Camera").mClickedEvent += ActorWithComponentCreationHandler<Core::Core_CCamera>(pParent, p_onItemClicked);
}