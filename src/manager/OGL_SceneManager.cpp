#include <memory>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "common/OGL_AssimpGLMHelpers.h"
#include "platform/windows/OGL_Application.h"
#include "manager/OGL_AnimationManager.h"
#include "manager/OGL_AssetLoader.h"
#include "sceneGraph/OGL_Scene.h"
#include "sceneGraph/OGL_Entity.h"
#include "sceneGraph/OGL_Mesh.h"
#include "sceneGraph/OGL_Light.h"
#include "sceneGraph/OGL_Camera.h"
#include "sceneGraph/OGL_Texture.h"
#include "sceneGraph/OGL_Material.h"
#include "sceneGraph/OGL_Transform.h"
#include "sceneGraph/OGL_Animator.h"
#include "sceneGraph/OGL_Animation.h"
#include "sceneGraph/OGL_MeshRenderer.h"
#include "OGL_SceneManager.h"

using namespace OGL;

bool OGL_SceneManager::Initialize()
{
    return true;
}

void OGL_SceneManager::Finalize()
{
}

void OGL_SceneManager::Tick(double dt)
{
	const auto& scene = mScenes.top();
	if (scene)
	{
		scene->Tick(dt);
	}
}

bool OGL_SceneManager::LoadScene(const std::string& sceneName)
{
	if (sceneName.length() > 0)
	{
		OGL_AssetLoader assetLoader;
		auto buffer = assetLoader.SyncOpenAndReadBinary(sceneName.c_str());
		Assimp::Importer importer;
		const aiScene* _aiScene = importer.ReadFile("./../../../" + sceneName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!_aiScene || !_aiScene->mRootNode)
		{
			std::cout << "Error::Assimp: " << importer.GetErrorString() << std::endl;
			return false;
		}

		Ref<OGL_Scene> scenePtr = CreateRef<OGL_Scene>();
		mScenes.push(scenePtr);

		//
		for (size_t i = 0; i < _aiScene->mNumLights; i++)
		{
			auto _aiLight = _aiScene->mLights[i];
			auto keyName = _aiLight->mName.C_Str();
			auto oglEntity = scenePtr->CreateEntity(keyName);
			auto& oglLight = oglEntity->AddComponent<OGL_Light>();

			oglLight.mUp = OGL_AssimpGLMHelpers::GetGLMVec(_aiLight->mUp);
			oglLight.mSize = OGL_AssimpGLMHelpers::GetGLMVec(_aiLight->mSize);
			oglLight.mDirection = OGL_AssimpGLMHelpers::GetGLMVec(_aiLight->mDirection);
			oglLight.mColorDiffuse = OGL_AssimpGLMHelpers::GetGLMColor(_aiLight->mColorDiffuse);
			oglLight.mColorSpecular = OGL_AssimpGLMHelpers::GetGLMColor(_aiLight->mColorSpecular);
			oglLight.mColorAmbient = OGL_AssimpGLMHelpers::GetGLMColor(_aiLight->mColorAmbient);

			oglLight.mAngleInnerCone = _aiLight->mAngleInnerCone;
			oglLight.mAngleOuterCone = _aiLight->mAngleOuterCone;
			oglLight.mAttenuationLinear = _aiLight->mAttenuationLinear;
			oglLight.mAttenuationConstant = _aiLight->mAttenuationConstant;
			oglLight.mAttenuationQuadratic = _aiLight->mAttenuationQuadratic;
		}

		//
		for (size_t i = 0; i < _aiScene->mNumCameras; i++)
		{
			auto _aiCamera = _aiScene->mCameras[i];
			auto keyName = _aiCamera->mName.C_Str();
			auto oglEntity = scenePtr->CreateEntity(keyName);
			auto& oglCamera = oglEntity->AddComponent<OGL_Camera>();

			oglCamera.mUp = OGL_AssimpGLMHelpers::GetGLMVec(_aiCamera->mUp);
			oglCamera.mLookAt = OGL_AssimpGLMHelpers::GetGLMVec(_aiCamera->mLookAt);

			oglCamera.mClipPlaneFar = _aiCamera->mClipPlaneFar;
			oglCamera.mClipPlaneNear = _aiCamera->mClipPlaneNear;
			oglCamera.mHorizontalFOV = _aiCamera->mHorizontalFOV;
			oglCamera.mOrthographicWidth = _aiCamera->mOrthographicWidth;
		}

		std::function<Ref<OGL_Mesh>(aiMesh* mesh)> _ProcessMesh;
		std::function<void(const aiScene* scene, aiNode* node)> _ProcessNode;
		std::function<Ref<OGL_Texture>(const aiMaterial* material, aiTextureType type)> _ProcessTexture;

		_ProcessMesh = [&](aiMesh* mesh) -> Ref<OGL_Mesh>
		{
			auto keyName = mesh->mName.C_Str();
			auto oglMesh = CreateRef<OGL_Mesh>(keyName);
			oglMesh->mPrimitiveTypes = mesh->mPrimitiveTypes;

			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;

				vertex.position = OGL_AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);

				if (mesh->HasNormals())
				{
					oglMesh->hasNormal = true;
					vertex.normal = OGL_AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
				}

				if (mesh->HasTangentsAndBitangents())
				{
					oglMesh->hasTangentsAndBitangents = true;
					vertex.tangent = OGL_AssimpGLMHelpers::GetGLMVec(mesh->mTangents[i]);
					vertex.bitangent = OGL_AssimpGLMHelpers::GetGLMVec(mesh->mBitangents[i]);
				}

				if (mesh->HasTextureCoords(0))
				{
					oglMesh->hasTextureCoords = true;
					vertex.texcoord = OGL_AssimpGLMHelpers::GetGLMVec(mesh->mTextureCoords[0][i]);
				}
				else
				{
					vertex.texcoord = glm::vec2(0);
				}

				oglMesh->mVertices.push_back(vertex);
				oglMesh->SetVertexBoneDataToDefault(vertex);
			}

			for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (uint32_t j = 0; j < face.mNumIndices; j++)
				{
					oglMesh->mIndices.push_back(face.mIndices[j]);
				}
			}

			return oglMesh;
		};

		_ProcessTexture = [&](const aiMaterial* material, aiTextureType type) -> Ref<OGL_Texture>
		{

			for (GLuint i = 0; i < material->GetTextureCount(type); i++)
			{
				aiString str;
				material->GetTexture(type, i, &str);
				return CreateRef<OGL_Texture>(str.C_Str());
			}

			return nullptr;
		};

		_ProcessNode = [&](const aiScene* scene, aiNode* node) 
		{
			auto transform = node->mTransformation;
			auto tempTransform = OGL_AssimpGLMHelpers::ConvertMatrixToGLMFormat(transform);
			auto keyName = node->mName.C_Str();
			auto oglEntity = scenePtr->GetEntity(keyName);
			if (oglEntity)
			{
				auto& oglTransform = oglEntity->AddComponent<OGL_Transform>();
				oglTransform.mTransform = tempTransform;
			}
			else
			{
				if (node->mNumMeshes > 0)
				{
					oglEntity = scenePtr->CreateEntity(keyName);
					auto& oglTransform = oglEntity->AddComponent<OGL_Transform>();
					oglTransform.mTransform = tempTransform;

					auto& oglMashRenderer = oglEntity->AddComponent<OGL_MeshRenderer>();
					for (size_t i = 0; i < node->mNumMeshes; i++)
					{
						//
						auto _aiMesh = scene->mMeshes[node->mMeshes[i]];

						auto oglMesh = _ProcessMesh(_aiMesh);
						oglMashRenderer.mMeshs.push_back(oglMesh);

						for (int boneIndex = 0; boneIndex < _aiMesh->mNumBones; ++boneIndex)
						{
							int boneID = -1;
							std::string boneName = _aiMesh->mBones[boneIndex]->mName.C_Str();
							if (oglMesh->mBoneInfoMap.find(boneName) == oglMesh->mBoneInfoMap.end())
							{
								BoneInfo newBoneInfo;
								newBoneInfo.id = oglMesh->mBoneCounter;
								newBoneInfo.offset = OGL_AssimpGLMHelpers::ConvertMatrixToGLMFormat(_aiMesh->mBones[boneIndex]->mOffsetMatrix);
								oglMesh->mBoneInfoMap[boneName] = newBoneInfo;
								boneID = oglMesh->mBoneCounter;
								oglMesh->mBoneCounter++;
							}
							else
							{
								boneID = oglMesh->mBoneInfoMap[boneName].id;
							}
							auto weights = _aiMesh->mBones[boneIndex]->mWeights;
							int numWeights = _aiMesh->mBones[boneIndex]->mNumWeights;

							for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
							{
								int vertexId = weights[weightIndex].mVertexId;
								float weight = weights[weightIndex].mWeight;
								oglMesh->SetVertexBoneData(oglMesh->mVertices[vertexId], boneID, weight);
							}
						}

						//
						auto _aiMaterial = scene->mMaterials[_aiMesh->mMaterialIndex];

						auto oglMaterial = CreateRef<OGL_Material>();

						aiString name;
						if (AI_SUCCESS == aiGetMaterialString(_aiMaterial, AI_MATKEY_NAME, &name))
						{
							oglMaterial->mName = name.C_Str();
						}

						oglMaterial->mDiffuseMap = _ProcessTexture(_aiMaterial, aiTextureType_DIFFUSE);
						oglMaterial->mNormalMap = _ProcessTexture(_aiMaterial, aiTextureType_NORMALS);
						oglMaterial->mLightMap = _ProcessTexture(_aiMaterial, aiTextureType_LIGHTMAP);

						auto keyName = _aiMesh->mName.C_Str();
						oglMashRenderer.mMaterials.emplace(keyName, oglMaterial);
					}

					//
					auto app = static_cast<OGL_Application*>(mApp);
					auto animationName = node->mParent->mName.C_Str();
					if (app->mAnimationManager->mAnimations[animationName])
					{
						auto& oglAnimator = oglEntity->AddComponent<OGL_Animator>();
						oglAnimator.mCurrentAnimation = app->mAnimationManager->mAnimations[animationName];
						oglAnimator.mCurrentAnimation->InitBones(oglMashRenderer);
					}
				}

				for (size_t i = 0; i < node->mNumChildren; i++)
				{
					_ProcessNode(scene, node->mChildren[i]);
				}
			}
		};

		_ProcessNode(_aiScene, _aiScene->mRootNode);
	}

	mSceneRevision++;

    return true;
}

const Ref<OGL_Scene> OGL_SceneManager::GetSceneForRendering() const
{
    return mScenes.top();
}
