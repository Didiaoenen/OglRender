#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/OGL_Base.h"
#include "common/OGL_Define.h"
#include "sceneGraph/OGL_Bone.h"
#include "OGL_Asset.h"

namespace OGL
{

class OGL_Bone;
class OGL_MeshRenderer;

struct AnimationTranslate
{
	std::vector<KeyPosition> mPositions;
	std::vector<KeyRotation> mRotations;
	std::vector<KeyScale> mScales;
};

class OGL_Animation : public OGL_Asset
{
public:
	OGL_Animation() = default;
	~OGL_Animation() = default;

	bool InitBones(const OGL_MeshRenderer& meshRenderer);
	Ref<OGL_Bone> FindBone(const std::string& name);

	auto& GetAnimationTranslate() { return mAniamtionTranslateMap; }

public:
	BoneNode mRootNode;
	float mDuration{ 0.0 };
	int mTicksPerSecond{ 0 };
	std::vector<Ref<OGL_Bone>> mBones;
	std::map<std::string, BoneInfo> mBoneInfoMap;
	std::map<std::string, AnimationTranslate> mAniamtionTranslateMap;

};
}

