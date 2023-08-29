#pragma once

#include <string>
#include <entt/entt.hpp>

#include "base/OGL_Base.h"
#include "sceneGraph/OGL_UUID.h"

namespace OGL
{

class OGL_Entity;

class OGL_Component
{
public:
	OGL_Component() = default;
	virtual ~OGL_Component() = default;

	virtual void Tick(double dt) {}

public:
	Ref<OGL_Entity> mEntity{ nullptr };

};

class OGL_IDComponent : public OGL_Component
{
public:
	OGL_IDComponent() = default;
	OGL_IDComponent(const OGL_UUID& uuid) : mID(uuid) {}
	virtual ~OGL_IDComponent() = default;

public:
	OGL_UUID mID;
};

class OGL_TagComponent : public OGL_Component
{
public:
	OGL_TagComponent() = default;
	OGL_TagComponent(const std::string& tag) : mTag(tag) {}
	virtual ~OGL_TagComponent() = default;

public:
	std::string mTag;
};

}

