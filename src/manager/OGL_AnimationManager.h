#pragma once

#include <map>
#include <string>

#include "base/OGL_Base.h"
#include "common/OGL_Define.h"
#include "interface/OGL_IRuntimeModule.h"

namespace OGL
{

class OGL_Mesh;
class OGL_Animation;

class OGL_AnimationManager : public OGL_IRuntimeModule
{
public:
	OGL_AnimationManager() = default;
	~OGL_AnimationManager() = default;

	bool Initialize() override;
	void Finalize() override;

	void Tick() override;

	bool LoadAnimation(const std::string& animationName, Ref<OGL_Mesh> mesh);

public:
	std::map<std::string, Ref<OGL_Animation>> mAnimations;

};
}

