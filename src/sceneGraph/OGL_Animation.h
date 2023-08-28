#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/OGL_Base.h"
#include "common/OGL_Define.h"

namespace OGL
{

class OGL_Mesh;
class OGL_Bone;

class OGL_Animation
{
public:
	OGL_Animation() = default;
	~OGL_Animation() = default;

	Ref<OGL_Bone> FindBone(const std::string& name);

	inline float GetDuration() { return mDuration; }
	inline float GetTicksPerSecond() { return mTicksPerSecond; }
	inline const BoneNode& GetRootNode() { return mRootNode; }

public:
	BoneNode mRootNode;
	float mDuration{ 0.0 };
	int mTicksPerSecond{ 0 };
	std::vector<Ref<OGL_Bone>> mBones;
	std::map<std::string, BoneInfo> mBoneInfoMap;

};
}

