#pragma once

#include <vector>

#include "base/OGL_Base.h"
#include "common/OGL_Define.h"
#include "OGL_Component.h"

namespace OGL
{

class OGL_Animation;

class OGL_Animator : public OGL_Component
{
public:
	OGL_Animator();
	OGL_Animator(Ref<OGL_Animation> animation);
	OGL_Animator(const std::string& animationName);
	~OGL_Animator() = default;

	bool Init();

	void Tick(double dt) override;

	void PlayAnimation(Ref<OGL_Animation> animation);

	void CalculateBoneTransform(const BoneNode& node, glm::mat4 parentTransform);

public:
	std::string mCurrentAnimationName;
	std::vector<glm::mat4> mFinalBoneMatrices;
	Ref<OGL_Animation> mCurrentAnimation;
	float mCurrentTime{ 0.0 };
	float mDeltaTime{ 0.0 };

};
}

