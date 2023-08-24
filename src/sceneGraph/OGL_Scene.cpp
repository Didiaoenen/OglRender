#include "sceneGraph/OGL_Entity.h"
#include "sceneGraph/OGL_Component.h"
#include "OGL_Scene.h"

using namespace OGL;

Ref<OGL_Entity> OGL_Scene::CreateEntity(const std::string& name)
{
    auto entity = CreateRef<OGL_Entity>(mRegistry.create(), this);
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

void OGL_Scene::Tick()
{

}
