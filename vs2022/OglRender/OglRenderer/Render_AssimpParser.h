#pragma once

#include <vector>

#include "Render_Vertex.h"
#include "Render_IModelParser.h"

namespace Render
{
	class Render_AssimpParser : public Render_IModelParser
	{
	public:
		bool LoadModel
		(
			const std::string& p_fileName,
			std::vector<Render_Mesh*>& p_meshes,
			std::vector<std::string>& pMaterials,
			EModelParserFlags pParserFlags
		) override;

	private:
		void ProcessMaterials(const struct aiScene* pScene, std::vector<std::string>& pMaterials);;
		void ProcessNode(void* pTransform, struct aiNode* pNode, const struct aiScene* pScene, std::vector<Render_Mesh*>& p_meshes);
		void ProcessMesh(void* pTransform, struct aiMesh* pMesh, const struct aiScene* pScene, std::vector<Render_Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices);
	};
}

