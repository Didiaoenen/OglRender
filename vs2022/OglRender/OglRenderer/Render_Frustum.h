#pragma once

#include <array>
#include <glm/glm.hpp>

#include <OglMaths/Maths_FTransform.h>
#include <OglMaths/Maths_FQuaternionExt.h>

#include "Render_BoundingSphere.h"

namespace Render
{
	class Render_Frustum
	{
	public:
		void CalculateFrustum(const glm::mat4& pViewProjection);

		bool PointInFrustum(float pX, float pY, float pZ) const;

		bool SphereInFrustum(float pX, float pY, float pZ, float pRadius) const;

		bool CubeInFrustum(float pX, float pY, float pZ, float pSize) const;

		bool BoundingSphereInFrustum(const Render_BoundingSphere& pBoundingSphere, const Maths::Maths_FTransform& pTransform) const;

		std::array<float, 4> GetNearPlane() const;

		std::array<float, 4> GetFarPlane() const;

	private:
		float mFrustum[6][4];
	};
}

