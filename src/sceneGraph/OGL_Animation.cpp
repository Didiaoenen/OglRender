
#include "sceneGraph/OGL_Mesh.h"
#include "sceneGraph/OGL_MeshRenderer.h"
#include "OGL_Animation.h"

using namespace OGL;

bool OGL_Animation::InitBones(const OGL_MeshRenderer& meshRenderer)
{
	if (mInited)
	{
		return mInited;
	}

	for (size_t i = 0; i < meshRenderer.mMeshs.size(); i++)
	{
		const auto& oglMesh = meshRenderer.mMeshs[i];
		for (const auto& [name, boneInfo] : oglMesh->mBoneInfoMap)
		{
			const auto& it = mAniamtionTranslateMap.find(name);
			if (it != mAniamtionTranslateMap.end())
			{
				const auto& [name, translate] = *it;
				auto oglBone = CreateRef<OGL_Bone>(name, boneInfo.id);
				oglBone->mPositions = translate.mPositions;
				oglBone->mNumPositions = translate.mPositions.size();
				oglBone->mRotations = translate.mRotations;
				oglBone->mNumRotations = translate.mRotations.size();
				oglBone->mScales = translate.mScales;
				oglBone->mNumScalings = translate.mScales.size();
				mBones.push_back(oglBone);
			}
		}
	}

	mInited = true;

	return mInited;
}

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
