#pragma once

#include <entt/entt.hpp>

#include "base/OGL_Base.h"
#include "base/OGL_RefCounts.h"
#include "sceneGraph/OGL_Scene.h"
#include "sceneGraph/OGL_UUID.h"

namespace OGL
{

class OGL_Scene;
class OGL_IDComponent;

class OGL_Entity : public OGL_RefCounts
{
public:
	OGL_Entity();
	OGL_Entity(entt::entity handle, OGL_Scene* scene, const std::string& mName);
	OGL_Entity(const OGL_Entity& other);
	virtual ~OGL_Entity() = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		auto& component = mScene->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
		component.mEntity = CreateRef<OGL_Entity>(*this);
		return component;
	}

	template<typename T>
	T& GetComponent()
	{
		return mScene->mRegistry.get<T>(mEntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return mScene->mRegistry.any_of<T>(mEntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		mScene->mRegistry.remove<T>(mEntityHandle);
	}

	operator bool() const { return mEntityHandle != entt::null; }
	operator uint32_t() const { return (uint32_t)mEntityHandle; }
	operator entt::entity() const { return mEntityHandle; }

	OGL_UUID GetUUID();
	const std::string& GetName();


public:
	std::string mName{ std::string() };

	OGL_Scene* mScene{ nullptr };
	entt::entity mEntityHandle{ entt::null };
};
}

