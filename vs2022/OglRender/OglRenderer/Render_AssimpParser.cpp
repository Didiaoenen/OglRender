#include "Render_AssimpParser.h"

bool Render::Render_AssimpParser::LoadModel(const std::string& p_fileName, std::vector<Render_Mesh*>& p_meshes, std::vector<std::string>& p_materials, EModelParserFlags p_parserFlags)
{
    return false;
}

void Render::Render_AssimpParser::ProcessMaterials(const aiScene* p_scene, std::vector<std::string>& p_materials)
{
}

void Render::Render_AssimpParser::ProcessNode(void* p_transform, aiNode* pNode, const aiScene* p_scene, std::vector<Render_Mesh*>& p_meshes)
{
}

void Render::Render_AssimpParser::ProcessMesh(void* p_transform, aiMesh* p_mesh, const aiScene* p_scene, std::vector<Render_Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices)
{
}
