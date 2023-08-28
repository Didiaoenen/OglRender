
#include "sceneGraph/OGL_Bone.h"
#include "sceneGraph/OGL_Animation.h"
#include "OGL_Animator.h"

using namespace OGL;

OGL_Animator::OGL_Animator(Ref<OGL_Animation> animation)
	: mCurrentTime(0.0), mCurrentAnimation(animation)
{
	mFinalBoneMatrices.reserve(MAX_BONE);

	for (int i = 0; i < MAX_BONE; i++)
	{
		mFinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
}

void OGL_Animator::Tick(float dt)
{
	mDeltaTime = dt;
	if (mCurrentAnimation)
	{
		mCurrentTime += mCurrentAnimation->mTicksPerSecond * dt;
		mCurrentTime = fmod(mCurrentTime, mCurrentAnimation->mDuration);
		CalculateBoneTransform(mCurrentAnimation->mRootNode, glm::mat4(1.0f));
	}
}

void OGL_Animator::PlayAnimation(Ref<OGL_Animation> animation)
{
	mCurrentAnimation = animation;
	mCurrentTime = 0.0f;
}

void OGL_Animator::CalculateBoneTransform(const BoneNode& node, glm::mat4 parentTransform)
{
	std::string nodeName = node.name;
	glm::mat4 nodeTransform = node.transformation;

	auto bone = mCurrentAnimation->FindBone(nodeName);

	if (bone)
	{
		bone->Tick(mCurrentTime);
		nodeTransform = bone->mLocalTransform;
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = mCurrentAnimation->mBoneInfoMap;
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		mFinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node.childrenCount; i++)
	{
		CalculateBoneTransform(node.children[i], globalTransformation);
	}
}
