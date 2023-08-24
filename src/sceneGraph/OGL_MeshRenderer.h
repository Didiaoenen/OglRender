#pragma once

#include <vector>
#include <unordered_map>

#include "sceneGraph/OGL_Component.h"
#include "base/OGL_Base.h"

namespace OGL
{

class OGL_Mesh;
class OGL_Material;

class OGL_MeshRenderer : public OGL_Component
{
public:
	OGL_MeshRenderer() = default;
	virtual ~OGL_MeshRenderer() = default;

public:
	std::vector<Ref<OGL_Mesh>> mMeshs;
	std::unordered_map<std::string, Ref<OGL_Material>> mMaterials;
};
}

