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
			std::vector<std::string>& p_materials,
			EModelParserFlags p_parserFlags
		) override;

	private:
		void ProcessMaterials(const struct aiScene* p_scene, std::vector<std::string>& p_materials);;
		void ProcessNode(void* p_transform, struct aiNode* pNode, const struct aiScene* p_scene, std::vector<Render_Mesh*>& p_meshes);
		void ProcessMesh(void* p_transform, struct aiMesh* p_mesh, const struct aiScene* p_scene, std::vector<Render_Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices);
	};
}

