
#include "sceneGraph/OGL_Entity.h"
#include "sceneGraph/OGL_Animator.h"
#include "sceneGraph/OGL_Component.h"
#include "OGL_Scene.h"

#include "tracy/tracy/Tracy.hpp"

using namespace OGL;

Ref<OGL_Entity> OGL_Scene::CreateEntity(const std::string& name)
{
    auto entity = CreateRef<OGL_Entity>(mRegistry.create(), this, name);
    entity->AddComponent<OGL_IDComponent>(OGL_UUID());
    entity->AddComponent<OGL_TagComponent>(name);
    mEntitys.emplace(name, entity);
    return entity;
}

Ref<OGL_Entity> OGL_Scene::GetEntity(const std::string& name)
{
    auto entity = mEntitys.find(name);
    if (entity != mEntitys.end())
    {
        return entity->second;
    }
    return nullptr;
}

void OGL_Scene::Tick(double dt)
{
    ZoneScoped;

    auto view = mRegistry.view<OGL_Animator>();
    for (auto& entity : view)
    {
        auto& animator = view.get<OGL_Animator>(entity);
        animator.Tick(dt);
    }
}
