#pragma once

#include <vector>
#include <memory>

#include "Render_IMesh.h"
#include "Render_Vertex.h"
#include "Render_VertexArray.h"
#include "Render_IndexBuffer.h"
#include "Render_BoundingSphere.h"

namespace Render
{
	class Render_Mesh : public Render_IMesh
	{
	public:
		Render_Mesh(const std::vector<Render_Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex);

		virtual void Bind() override;

		virtual void Unbind() override;

		virtual uint32_t GetVertexCount() override;

		virtual uint32_t GetIndexCount() override;

		uint32_t GetMaterialIndex() const;

		const Render_BoundingSphere& GetBoundingSphere() const;

	private:
		void CreateBuffers(const std::vector<Render_Vertex>& p_vertices, const std::vector<uint32_t>& p_indices);
		void ComputeBoundingSphere(const std::vector<Render_Vertex>& p_vertices);

	private:
		const uint32_t m_vertexCount;
		const uint32_t m_indicesCount;
		const uint32_t m_materialIndex;

		Render_VertexArray							m_vertexArray;
		std::unique_ptr<Render_VertexBuffer<float>>	m_vertexBuffer;
		std::unique_ptr<Render_IndexBuffer>			m_indexBuffer;

		Render_BoundingSphere m_boundingSphere;
	};
}

