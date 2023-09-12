#pragma once

#include <array>
#include <glm/glm.hpp>

#include <OglMaths/Maths_FTransform.h>

#include "Render_BoundingSphere.h"

namespace Render
{
	class Render_Frustum
	{
	public:
		void CalculateFrustum(const glm::mat4& p_viewProjection);

		bool PointInFrustum(float p_x, float p_y, float p_z) const;

		bool SphereInFrustum(float p_x, float p_y, float p_z, float pRadius) const;

		bool CubeInFrustum(float p_x, float p_y, float p_z, float pSize) const;

		bool BoundingSphereInFrustum(const Render_BoundingSphere& pBoundingSphere, const Maths::Maths_FTransform& p_transform) const;

		std::array<float, 4> GetNearPlane() const;

		std::array<float, 4> GetFarPlane() const;

	private:
		float m_frustum[6][4];
	};
}

