#include <memory>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "manager/OGL_AssetLoader.h"
#include "sceneGraph/OGL_Scene.h"
#include "sceneGraph/OGL_Entity.h"
#include "sceneGraph/OGL_Mesh.h"
#include "sceneGraph/OGL_Light.h"
#include "sceneGraph/OGL_Camera.h"
#include "sceneGraph/OGL_Texture.h"
#include "sceneGraph/OGL_Material.h"
#include "sceneGraph/OGL_Transform.h"
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

void OGL_SceneManager::Tick()
{
}

bool OGL_SceneManager::LoadScene(const std::string& sceneName)
{
	if (sceneName.length() > 0)
	{
		OGL_AssetLoader assetLoader;
		auto buffer = assetLoader.SyncOpenAndReadBinary(sceneName.c_str());
		Assimp::Importer importer;
		const aiScene* _aiScene = importer.ReadFile("./../../../" + sceneName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!_aiScene || _aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !_aiScene->mRootNode)
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

			oglLight.mUp.x = _aiLight->mUp.x;
			oglLight.mUp.y = _aiLight->mUp.y;
			oglLight.mUp.z = _aiLight->mUp.z;

			oglLight.mSize.x = _aiLight->mSize.x;
			oglLight.mSize.y = _aiLight->mSize.y;

			oglLight.mDirection.x = _aiLight->mDirection.x;
			oglLight.mDirection.y = _aiLight->mDirection.y;
			oglLight.mDirection.z = _aiLight->mDirection.z;

			oglLight.mColorDiffuse.r = _aiLight->mColorDiffuse.r;
			oglLight.mColorDiffuse.g = _aiLight->mColorDiffuse.g;
			oglLight.mColorDiffuse.b = _aiLight->mColorDiffuse.b;

			oglLight.mColorSpecular.r = _aiLight->mColorSpecular.r;
			oglLight.mColorSpecular.g = _aiLight->mColorSpecular.g;
			oglLight.mColorSpecular.b = _aiLight->mColorSpecular.b;

			oglLight.mColorAmbient.r = _aiLight->mColorAmbient.r;
			oglLight.mColorAmbient.g = _aiLight->mColorAmbient.g;
			oglLight.mColorAmbient.b = _aiLight->mColorAmbient.b;

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

			oglCamera.mUp.x = _aiCamera->mUp.x;
			oglCamera.mUp.y = _aiCamera->mUp.y;
			oglCamera.mUp.z = _aiCamera->mUp.z;

			oglCamera.mLookAt.x = _aiCamera->mLookAt.x;
			oglCamera.mLookAt.y = _aiCamera->mLookAt.y;
			oglCamera.mLookAt.z = _aiCamera->mLookAt.z;

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

				vertex.position.x = mesh->mVertices[i].x;
				vertex.position.y = mesh->mVertices[i].y;
				vertex.position.z = mesh->mVertices[i].z;

				if (mesh->HasNormals())
				{
					oglMesh->hasNormal = true;

					vertex.normal.x = mesh->mNormals[i].x;
					vertex.normal.y = mesh->mNormals[i].y;
					vertex.normal.z = mesh->mNormals[i].z;
				}

				if (mesh->HasTangentsAndBitangents())
				{
					oglMesh->hasTangentsAndBitangents = true;

					vertex.tangent.x = mesh->mTangents[i].x;
					vertex.tangent.y = mesh->mTangents[i].y;
					vertex.tangent.z = mesh->mTangents[i].z;

					vertex.bitangent.x = mesh->mBitangents[i].x;
					vertex.bitangent.y = mesh->mBitangents[i].y;
					vertex.bitangent.z = mesh->mBitangents[i].z;
				}

				if (mesh->HasTextureCoords(0))
				{
					oglMesh->hasTextureCoords = true;

					vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
					vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
				}
				else
				{
					vertex.texcoord = glm::vec2(0);
				}

				oglMesh->mVertices.push_back(vertex);
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
			auto tempTransform = glm::identity<glm::mat4>();

			tempTransform[0][0] = transform.a1;
			tempTransform[0][1] = transform.a2;
			tempTransform[0][2] = transform.a3;
			tempTransform[0][3] = transform.d1;

			tempTransform[1][0] = transform.b1;
			tempTransform[1][1] = transform.b2;
			tempTransform[1][2] = transform.b3;
			tempTransform[1][3] = transform.d2;

			tempTransform[2][0] = transform.c1;
			tempTransform[2][1] = transform.c2;
			tempTransform[2][2] = transform.c3;
			tempTransform[2][3] = transform.d3;

			tempTransform[3][0] = transform.a4;
			tempTransform[3][1] = transform.b4;
			tempTransform[3][2] = transform.c4;
			tempTransform[3][3] = transform.d4;

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
						auto _aiMesh = scene->mMeshes[node->mMeshes[i]];

						oglMashRenderer.mMeshs.push_back(_ProcessMesh(_aiMesh));

						auto _aiMaterial = scene->mMaterials[_aiMesh->mMaterialIndex];

						auto oglMaterial = CreateRef<OGL_Material>();

						aiString name;
						if (AI_SUCCESS == aiGetMaterialString(_aiMaterial, AI_MATKEY_NAME, &name))
						{
							oglMaterial->mName = name.C_Str();
						}

						oglMaterial->mDiffuseMap = _ProcessTexture(_aiMaterial, aiTextureType_DIFFUSE);
						oglMaterial->mNormalMap = _ProcessTexture(_aiMaterial, aiTextureType_NORMALS);
						oglMaterial->mMaskMap = _ProcessTexture(_aiMaterial, aiTextureType_LIGHTMAP);

						auto keyName = _aiMesh->mName.C_Str();
						oglMashRenderer.mMaterials.emplace(keyName, oglMaterial);
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
