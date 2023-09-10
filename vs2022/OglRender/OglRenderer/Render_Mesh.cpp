#include "Render_Mesh.h"

Render::Render_Mesh::Render_Mesh(const std::vector<Render_Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex) :
	m_vertexCount(static_cast<uint32_t>(p_vertices.size())),
	m_indicesCount(static_cast<uint32_t>(p_indices.size())),
	m_materialIndex(p_materialIndex)
{
}

void Render::Render_Mesh::Bind()
{
}

void Render::Render_Mesh::Unbind()
{
}

uint32_t Render::Render_Mesh::GetVertexCount()
{
	return 0;
}

uint32_t Render::Render_Mesh::GetIndexCount()
{
	return 0;
}

uint32_t Render::Render_Mesh::GetMaterialIndex() const
{
	return 0;
}

const Render::Render_BoundingSphere& Render::Render_Mesh::GetBoundingSphere() const
{
	return m_boundingSphere;
}

void Render::Render_Mesh::CreateBuffers(const std::vector<Render_Vertex>& p_vertices, const std::vector<uint32_t>& p_indices)
{
}

void Render::Render_Mesh::ComputeBoundingSphere(const std::vector<Render_Vertex>& p_vertices)
{
}
