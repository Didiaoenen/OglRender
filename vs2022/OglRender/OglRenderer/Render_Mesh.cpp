#include "Render_Mesh.h"

Render::Render_Mesh::Render_Mesh(const std::vector<Render_Vertex>& pVertices, const std::vector<uint32_t>& pIndices, uint32_t pMaterialIndex) :
	mVertexCount(static_cast<uint32_t>(pVertices.size())),
	mIndicesCount(static_cast<uint32_t>(pIndices.size())),
	mMaterialIndex(pMaterialIndex)
{
	CreateBuffers(pVertices, pIndices);
	ComputeBoundingSphere(pVertices);
}

void Render::Render_Mesh::Bind()
{
	mVertexArray.Bind();
}

void Render::Render_Mesh::Unbind()
{
	mVertexArray.Unbind();
}

uint32_t Render::Render_Mesh::GetVertexCount()
{
	return mVertexCount;
}

uint32_t Render::Render_Mesh::GetIndexCount()
{
	return mIndicesCount;
}

uint32_t Render::Render_Mesh::GetMaterialIndex() const
{
	return mMaterialIndex;
}

const Render::Render_BoundingSphere& Render::Render_Mesh::GetBoundingSphere() const
{
	return mBoundingSphere;
}

void Render::Render_Mesh::CreateBuffers(const std::vector<Render_Vertex>& pVertices, const std::vector<uint32_t>& pIndices)
{
	std::vector<float> vertexData;

	std::vector<unsigned int> rawIndices;

	for (const auto& vertex : pVertices)
	{
		vertexData.push_back(vertex.position[0]);
		vertexData.push_back(vertex.position[1]);
		vertexData.push_back(vertex.position[2]);

		vertexData.push_back(vertex.texCoords[0]);
		vertexData.push_back(vertex.texCoords[1]);

		vertexData.push_back(vertex.normals[0]);
		vertexData.push_back(vertex.normals[1]);
		vertexData.push_back(vertex.normals[2]);

		vertexData.push_back(vertex.tangent[0]);
		vertexData.push_back(vertex.tangent[1]);
		vertexData.push_back(vertex.tangent[2]);

		vertexData.push_back(vertex.bitangent[0]);
		vertexData.push_back(vertex.bitangent[1]);
		vertexData.push_back(vertex.bitangent[2]);
	}

	mVertexBuffer = std::make_unique<Render_VertexBuffer<float>>(vertexData);
	mIndexBuffer = std::make_unique<Render_IndexBuffer>(const_cast<uint32_t*>(pIndices.data()), pIndices.size());

	uint64_t vertexSize = sizeof(Render_Vertex);

	mVertexArray.BindAttribute(0, *mVertexBuffer, EType::FLOAT, 3, vertexSize, 0);
	mVertexArray.BindAttribute(1, *mVertexBuffer, EType::FLOAT, 2, vertexSize, sizeof(float) * 3);
	mVertexArray.BindAttribute(2, *mVertexBuffer, EType::FLOAT, 3, vertexSize, sizeof(float) * 5);
	mVertexArray.BindAttribute(3, *mVertexBuffer, EType::FLOAT, 3, vertexSize, sizeof(float) * 8);
	mVertexArray.BindAttribute(4, *mVertexBuffer, EType::FLOAT, 3, vertexSize, sizeof(float) * 11);
}

void Render::Render_Mesh::ComputeBoundingSphere(const std::vector<Render_Vertex>& pVertices)
{
	mBoundingSphere.position = glm::vec3(0);
	mBoundingSphere.radius = 0.0f;

	if (!pVertices.empty())
	{
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		float maxZ = std::numeric_limits<float>::min();

		for (const auto& vertex : pVertices)
		{
			minX = std::min(minX, vertex.position[0]);
			minY = std::min(minY, vertex.position[1]);
			minZ = std::min(minZ, vertex.position[2]);

			maxX = std::max(maxX, vertex.position[0]);
			maxY = std::max(maxY, vertex.position[1]);
			maxZ = std::max(maxZ, vertex.position[2]);
		}

		mBoundingSphere.position = glm::vec3{ minX + maxX, minY + maxY, minZ + maxZ } / 2.0f;

		for (const auto& vertex : pVertices)
		{
			const auto& position = reinterpret_cast<const glm::vec3&>(vertex.position);
			mBoundingSphere.radius = std::max(mBoundingSphere.radius, glm::distance(mBoundingSphere.position, position));
		}
	}
}
