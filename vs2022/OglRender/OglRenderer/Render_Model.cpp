
#include <algorithm>

#include "Render_Model.h"

const std::vector<Render::Render_Mesh*>& Render::Render_Model::GetMeshes() const
{
	return mMeshes;
}

const std::vector<std::string>& Render::Render_Model::GetMaterialNames() const
{
	return mMaterialNames;
}

const Render::Render_BoundingSphere& Render::Render_Model::GetBoundingSphere() const
{
	return mBoundingSphere;
}

Render::Render_Model::Render_Model(const std::string& pPath) :
	mPath(pPath)
{
}

Render::Render_Model::~Render_Model()
{
	for (auto mesh : mMeshes)
	{
		delete mesh;
	}
}

void Render::Render_Model::ComputeBoundingSphere()
{
	if (mMeshes.size() == 1)
	{
		mBoundingSphere = mMeshes[0]->GetBoundingSphere();
	}
	else
	{
		mBoundingSphere.position = glm::vec3(0.0f);
		mBoundingSphere.radius = 0.0f;

		if (!mMeshes.empty())
		{
			float minX = std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float minZ = std::numeric_limits<float>::max();

			float maxX = std::numeric_limits<float>::min();
			float maxY = std::numeric_limits<float>::min();
			float maxZ = std::numeric_limits<float>::min();

			for (const auto& mesh : mMeshes)
			{
				const auto& boundingSphere = mesh->GetBoundingSphere();
				minX = std::min(minX, boundingSphere.position.x - boundingSphere.radius);
				minY = std::min(minY, boundingSphere.position.y - boundingSphere.radius);
				minZ = std::min(minZ, boundingSphere.position.z - boundingSphere.radius);

				maxX = std::max(maxX, boundingSphere.position.x + boundingSphere.radius);
				maxY = std::max(maxY, boundingSphere.position.y + boundingSphere.radius);
				maxZ = std::max(maxZ, boundingSphere.position.z + boundingSphere.radius);
			}

			mBoundingSphere.position = glm::vec3{ minX + maxX, minY + maxY, minZ + maxZ } / 2.0f;
			mBoundingSphere.radius = glm::distance(mBoundingSphere.position, { minX, minY, minZ });
		}
	}
}
