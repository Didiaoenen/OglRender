#include <algorithm>

#include "Render_Frustum.h"

enum FrustumSide
{
	RIGHT = 0,
	LEFT = 1,
	BOTTOM = 2,
	TOP = 3,
	BACK = 4,
	FRONT = 5,
};

enum PlaneData
{
	A = 0,
	B = 1,
	C = 2,
	D = 3,
};

void NormalizePlane(float frustum[6][4], int side)
{
	float magnitude = (float)sqrt(frustum[side][A] * frustum[side][A] + frustum[side][B] * frustum[side][B] + frustum[side][C] * frustum[side][C]);

	frustum[side][A] /= magnitude;
	frustum[side][B] /= magnitude;
	frustum[side][C] /= magnitude;
	frustum[side][D] /= magnitude;
}

void Render::Render_Frustum::CalculateFrustum(const glm::mat4& pViewProjection)
{

}

bool Render::Render_Frustum::PointInFrustum(float p_x, float p_y, float p_z) const
{
	return false;
}

bool Render::Render_Frustum::SphereInFrustum(float p_x, float p_y, float p_z, float pRadius) const
{
	return false;
}

bool Render::Render_Frustum::CubeInFrustum(float p_x, float p_y, float p_z, float pSize) const
{
	return false;
}

bool Render::Render_Frustum::BoundingSphereInFrustum(const Render_BoundingSphere& pBoundingSphere, const Maths::Maths_FTransform& p_transform) const
{
	return false;
}

std::array<float, 4> Render::Render_Frustum::GetNearPlane() const
{
	return std::array<float, 4>();
}

std::array<float, 4> Render::Render_Frustum::GetFarPlane() const
{
	return std::array<float, 4>();
}
