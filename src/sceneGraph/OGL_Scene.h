#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <entt/entt.hpp>

#include "base/OGL_Base.h"
#include "sceneGraph/OGL_Material.h"

namespace OGL
{

class OGL_SkyBox;
class OGL_Entity;

class OGL_Scene
{
public:
	[[nodiscard]] Ref<OGL_Entity> CreateEntity(const std::string& name = std::string());

	[[nodiscard]] Ref<OGL_Entity> GetEntity(const std::string& name);

	void Tick(double dt);

public:
	entt::registry mRegistry{};

	std::unordered_map<std::string, std::shared_ptr<OGL_Entity>> mEntitys;

	std::shared_ptr<OGL_SkyBox> mSkyBox;

private:
	std::shared_ptr<OGL_Material> mDefaultMaterial = CreateRef<OGL_Material>("default");
};
}
