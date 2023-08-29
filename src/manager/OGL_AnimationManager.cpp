
#include <iostream>
#include <functional>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "common/OGL_AssimpGLMHelpers.h"
#include "sceneGraph/OGL_Bone.h"
#include "sceneGraph/OGL_Mesh.h"
#include "sceneGraph/OGL_Animation.h"
#include "OGL_AnimationManager.h"

using namespace OGL;

bool OGL_AnimationManager::Initialize()
{
	return false;
}

void OGL_AnimationManager::Finalize()
{
}

void OGL_AnimationManager::Tick(double dt)
{

}

bool OGL_AnimationManager::LoadAnimation(const std::string& animationName)
{
	Assimp::Importer importer;
	const aiScene* _aiScene = importer.ReadFile("./../../../" + animationName, aiProcess_Triangulate);
	if (!_aiScene || !_aiScene->mRootNode)
	{
		std::cout << "Error::Assimp: " << importer.GetErrorString() << std::endl;
		return false;
	}

	std::function<void(BoneNode&, const aiNode*)> _ProcessNode;

	_ProcessNode = [&](BoneNode& dest, const aiNode* src)
	{
		dest.name = src->mName.C_Str();
		dest.transformation = OGL_AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.childrenCount = src->mNumChildren;

		for (size_t i = 0; i < src->mNumChildren; i++)
		{
			BoneNode newData;
			_ProcessNode(newData, src->mChildren[i]);
			dest.children.push_back(newData);
		}
	};

	for (size_t i = 0; i < _aiScene->mNumAnimations; i++)
	{
		auto _aiAnimation = _aiScene->mAnimations[i];
		
		auto oglAnimation = CreateRef<OGL_Animation>();
		auto keyName = _aiAnimation->mName.C_Str();
		mAnimations.emplace(keyName, oglAnimation);
		
		oglAnimation->mDuration = _aiAnimation->mDuration;
		oglAnimation->mTicksPerSecond = _aiAnimation->mTicksPerSecond;

		_ProcessNode(oglAnimation->mRootNode, _aiScene->mRootNode);

		auto& boneInfoMap = oglAnimation->mBoneInfoMap;

		for (int i = 0; i < _aiAnimation->mNumChannels; i++)
		{
			auto channel = _aiAnimation->mChannels[i];
			std::string keyName = channel->mNodeName.C_Str();

			AnimationTranslate translate;

			//auto oglBone = CreateRef<OGL_Bone>(keyName, i);
			//oglBone->mNumPositions = channel->mNumPositionKeys;
			for (size_t i = 0; i < channel->mNumPositionKeys; i++)
			{
				auto _aiPosition = channel->mPositionKeys[i].mValue;
				float timeStamp = channel->mPositionKeys[i].mTime;
				KeyPosition data;
				data.position = AssimpGLMHelpers::GetGLMVec(_aiPosition);
				data.timeStamp = timeStamp;
				translate.mPositions.push_back(data);
			}

			//oglBone->mNumRotations = channel->mNumRotationKeys;
			for (size_t i = 0; i < channel->mNumRotationKeys; i++)
			{
				auto _aiOrientation = channel->mRotationKeys[i].mValue;
				float timeStamp = channel->mRotationKeys[i].mTime;
				KeyRotation data;
				data.orientation = AssimpGLMHelpers::GetGLMQuat(_aiOrientation);
				data.timeStamp = timeStamp;
				translate.mRotations.push_back(data);
			}

			//oglBone->mNumScalings = channel->mNumScalingKeys;
			for (size_t i = 0; i < channel->mNumScalingKeys; i++)
			{
				auto _aiScale = channel->mScalingKeys[i].mValue;
				float timeStamp = channel->mScalingKeys[i].mTime;
				KeyScale data;
				data.scale = AssimpGLMHelpers::GetGLMVec(_aiScale);
				data.timeStamp = timeStamp;
				translate.mScales.push_back(data);
			}

			oglAnimation->mAniamtionTranslateMap[keyName] = translate;

			//oglAnimation->mBones.push_back(oglBone);
		}
	}

	return true;
}
