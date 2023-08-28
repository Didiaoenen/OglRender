#pragma once

#include <vector>

#include "base/OGL_Base.h"
#include "common/OGL_Define.h"

namespace OGL
{

class OGL_Animation;

class OGL_Animator
{
public:
	OGL_Animator(Ref<OGL_Animation> animation);

	void Tick(float dt);

	void PlayAnimation(Ref<OGL_Animation> animation);

	void CalculateBoneTransform(const BoneNode& node, glm::mat4 parentTransform);

public:
	std::vector<glm::mat4> mFinalBoneMatrices;
	Ref<OGL_Animation> mCurrentAnimation;
	float mCurrentTime{ 0.0 };
	float mDeltaTime{ 0.0 };

};
}

