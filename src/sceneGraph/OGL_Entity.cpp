#include "sceneGraph/OGL_Component.h"
#include "OGL_Entity.h"

using namespace OGL;

OGL_Entity::OGL_Entity()
{
}

OGL_Entity::OGL_Entity(entt::entity handle, OGL_Scene* scene, const std::string& name)
	: mEntityHandle(handle), mScene(scene), mName(name)
{
}

OGL_Entity::OGL_Entity(const OGL_Entity& other)
{
	*this = other;
}

OGL_UUID OGL_Entity::GetUUID()
{
	return GetComponent<OGL_IDComponent>().mID;
}

const std::string& OGL_Entity::GetName()
{
	return GetComponent<OGL_TagComponent>().mTag;
}
