
#include "OGL_Bone.h"
#include "OGL_Animation.h"

using namespace OGL;

Ref<OGL_Bone> OGL_Animation::FindBone(const std::string& name)
{
	auto iter = std::find_if(mBones.begin(), mBones.end(),
		[&](const Ref<OGL_Bone> bone)
		{
			return bone->mName == name;
		}
	);

	if (iter != mBones.end())
	{
		return *iter;
	}
	return nullptr;
}
