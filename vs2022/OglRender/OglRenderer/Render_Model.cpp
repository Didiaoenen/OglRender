#include "Render_Model.h"

const std::vector<Render::Render_Mesh*>& Render::Render_Model::GetMeshes() const
{
	return m_meshes;
}

const std::vector<std::string>& Render::Render_Model::GetMaterialNames() const
{
	return mMaterialNames;
}

const Render::Render_BoundingSphere& Render::Render_Model::GetBoundingSphere() const
{
	return m_boundingSphere;
}

Render::Render_Model::Render_Model(const std::string& pPath) :
	path(pPath)
{
}

Render::Render_Model::~Render_Model()
{
}

void Render::Render_Model::ComputeBoundingSphere()
{
	if (m_meshes.size() == 1)
	{
		m_boundingSphere = m_meshes[0]->GetBoundingSphere();
	}
	else
	{
		m_boundingSphere.position = glm::vec3(0.0f);
		m_boundingSphere.radius = 0.0f;

		if (!m_meshes.empty())
		{
			float minX = std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float minZ = std::numeric_limits<float>::max();

			float maxX = std::numeric_limits<float>::min();
			float maxY = std::numeric_limits<float>::min();
			float maxZ = std::numeric_limits<float>::min();

			for (const auto& mesh : m_meshes)
			{
				const auto& boundingSphere = mesh->GetBoundingSphere();
				minX = std::min(minX, boundingSphere.position.x - boundingSphere.radius);
				minY = std::min(minY, boundingSphere.position.y - boundingSphere.radius);
				minZ = std::min(minZ, boundingSphere.position.z - boundingSphere.radius);

				maxX = std::max(maxX, boundingSphere.position.x + boundingSphere.radius);
				maxY = std::max(maxY, boundingSphere.position.y + boundingSphere.radius);
				maxZ = std::max(maxZ, boundingSphere.position.z + boundingSphere.radius);
			}

			m_boundingSphere.position = glm::vec3{ minX + maxX, minY + maxY, minZ + maxZ } / 2.0f;
			m_boundingSphere.radius = glm::distance(m_boundingSphere.position, { minX, minY, minZ });
		}
	}
}
