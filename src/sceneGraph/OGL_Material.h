#pragma once

#include <string>

#include "sceneGraph/OGL_Asset.h"
#include "common/OGL_ParameterValueMap.h"

namespace OGL
{
class OGL_Material : public OGL_Asset
{
public:
	OGL_Material() = default;
	OGL_Material(const std::string& name) : mName(name) {}
	virtual ~OGL_Material() = default;

public:
	std::string mName;
	Color mDiffuseMap;
	Color mNormalMap;
	Color mLightMap;
};
}