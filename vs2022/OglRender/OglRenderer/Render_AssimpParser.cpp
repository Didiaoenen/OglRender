
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

#include "Render_AssimpParser.h"

bool Render::Render_AssimpParser::LoadModel(const std::string& p_fileName, std::vector<Render_Mesh*>& p_meshes, std::vector<std::string>& pMaterials, EModelParserFlags pParserFlags)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(p_fileName, static_cast<unsigned int>(pParserFlags));

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return false;

	ProcessMaterials(scene, pMaterials);

	aiMatrix4x4 identity;

	ProcessNode(&identity, scene->mRootNode, scene, p_meshes);

	return true;
}

void Render::Render_AssimpParser::ProcessMaterials(const aiScene* pScene, std::vector<std::string>& pMaterials)
{
	for (uint32_t i = 0; i < pScene->mNumMaterials; ++i)
	{
		aiMaterial* material = pScene->mMaterials[i];
		if (material)
		{
			aiString name;
			aiGetMaterialString(material, AI_MATKEY_NAME, &name);
			pMaterials.push_back(name.C_Str());
		}
	}
}

void Render::Render_AssimpParser::ProcessNode(void* p_transform, aiNode* pNode, const aiScene* pScene, std::vector<Render_Mesh*>& p_meshes)
{
	aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(p_transform) * pNode->mTransformation;

	for (uint32_t i = 0; i < pNode->mNumMeshes; ++i)
	{
		std::vector<Render_Vertex> vertices;
		std::vector<uint32_t> indices;
		aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
		ProcessMesh(&nodeTransformation, mesh, pScene, vertices, indices);
		p_meshes.push_back(new Render_Mesh(vertices, indices, mesh->mMaterialIndex));
	}

	for (uint32_t i = 0; i < pNode->mNumChildren; ++i)
	{
		ProcessNode(&nodeTransformation, pNode->mChildren[i], pScene, p_meshes);
	}
}

void Render::Render_AssimpParser::ProcessMesh(void* p_transform, aiMesh* pMesh, const aiScene* pScene, std::vector<Render_Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices)
{
	aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(p_transform);

	for (uint32_t i = 0; i < pMesh->mNumVertices; ++i)
	{
		aiVector3D position = meshTransformation * pMesh->mVertices[i];
		aiVector3D normal = meshTransformation * (pMesh->mNormals ? pMesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
		aiVector3D texCoords = pMesh->mTextureCoords[0] ? pMesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D tangent = pMesh->mTangents ? meshTransformation * pMesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D bitangent = pMesh->mBitangents ? meshTransformation * pMesh->mBitangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);

		p_outVertices.push_back
		(
			{
				position.x,
				position.y,
				position.z,
				texCoords.x,
				texCoords.y,
				normal.x,
				normal.y,
				normal.z,
				tangent.x,
				tangent.y,
				tangent.z,
				bitangent.x,
				bitangent.y,
				bitangent.z
			}
		);
	}

	for (uint32_t faceID = 0; faceID < pMesh->mNumFaces; ++faceID)
	{
		auto& face = pMesh->mFaces[faceID];

		for (size_t indexID = 0; indexID < 3; ++indexID)
			p_outIndices.push_back(face.mIndices[indexID]);
	}
}
