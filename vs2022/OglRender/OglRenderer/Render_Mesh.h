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
		Render_Mesh(const std::vector<Render_Vertex>& pVertices, const std::vector<uint32_t>& pIndices, uint32_t pMaterialIndex);

		virtual void Bind() override;

		virtual void Unbind() override;

		virtual uint32_t GetVertexCount() override;

		virtual uint32_t GetIndexCount() override;

		uint32_t GetMaterialIndex() const;

		const Render_BoundingSphere& GetBoundingSphere() const;

	private:
		void CreateBuffers(const std::vector<Render_Vertex>& pVertices, const std::vector<uint32_t>& pIndices);
		void ComputeBoundingSphere(const std::vector<Render_Vertex>& pVertices);

	private:
		const uint32_t mVertexCount;
		const uint32_t mIndicesCount;
		const uint32_t mMaterialIndex;

		Render_VertexArray							mVertexArray;
		std::unique_ptr<Render_VertexBuffer<float>>	mVertexBuffer;
		std::unique_ptr<Render_IndexBuffer>			mIndexBuffer;

		Render_BoundingSphere mBoundingSphere;
	};
}

