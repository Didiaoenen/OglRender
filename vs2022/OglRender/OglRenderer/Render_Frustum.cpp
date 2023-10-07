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
	auto mat = glm::transpose(pViewProjection);

	mFrustum[RIGHT][A] = mat[0][3] - mat[0][0];
	mFrustum[RIGHT][B] = mat[1][3] - mat[1][0];
	mFrustum[RIGHT][C] = mat[2][3] - mat[2][0];
	mFrustum[RIGHT][D] = mat[3][3] - mat[3][0];

	NormalizePlane(mFrustum, RIGHT);

	mFrustum[LEFT][A] = mat[0][3] + mat[0][0];
	mFrustum[LEFT][B] = mat[1][3] + mat[1][0];
	mFrustum[LEFT][C] = mat[2][3] + mat[2][0];
	mFrustum[LEFT][D] = mat[3][3] + mat[3][0];

	NormalizePlane(mFrustum, LEFT);

	mFrustum[BOTTOM][A] = mat[0][3] + mat[0][1];
	mFrustum[BOTTOM][B] = mat[1][3] + mat[1][1];
	mFrustum[BOTTOM][C] = mat[2][3] + mat[2][1];
	mFrustum[BOTTOM][D] = mat[3][3] + mat[3][1];

	NormalizePlane(mFrustum, BOTTOM);

	mFrustum[TOP][A] = mat[0][3] - mat[0][1];
	mFrustum[TOP][B] = mat[1][3] - mat[1][1];
	mFrustum[TOP][C] = mat[2][3] - mat[2][1];
	mFrustum[TOP][D] = mat[3][3] - mat[3][1];

	NormalizePlane(mFrustum, TOP);

	mFrustum[BACK][A] = mat[0][3] - mat[0][2];
	mFrustum[BACK][B] = mat[1][3] - mat[1][2];
	mFrustum[BACK][C] = mat[2][3] - mat[2][2];
	mFrustum[BACK][D] = mat[3][3] - mat[3][2];

	NormalizePlane(mFrustum, BACK);

	mFrustum[FRONT][A] = mat[0][3] + mat[0][2];
	mFrustum[FRONT][B] = mat[1][3] + mat[1][2];
	mFrustum[FRONT][C] = mat[2][3] + mat[2][2];
	mFrustum[FRONT][D] = mat[3][3] + mat[3][2];

	NormalizePlane(mFrustum, FRONT);
}

bool Render::Render_Frustum::PointInFrustum(float pX, float pY, float pZ) const
{
	for (int i = 0; i < 6; i++)
	{
		if (mFrustum[i][A] * pX + mFrustum[i][B] * pY + mFrustum[i][C] * pZ + mFrustum[i][D] <= 0)
		{
			return false;
		}
	}

	return true;
}

bool Render::Render_Frustum::SphereInFrustum(float pX, float pY, float pZ, float pRadius) const
{
	for (int i = 0; i < 6; i++)
	{
		if (mFrustum[i][A] * pX + mFrustum[i][B] * pY + mFrustum[i][C] * pZ + mFrustum[i][D] <= -pRadius)
		{
			return false;
		}
	}

	return true;
}

bool Render::Render_Frustum::CubeInFrustum(float pX, float pY, float pZ, float pSize) const
{
	for (int i = 0; i < 6; i++)
	{
		if (mFrustum[i][A] * (pX - pSize) + mFrustum[i][B] * (pY - pSize) + mFrustum[i][C] * (pZ - pSize) + mFrustum[i][D] > 0)
			continue;
		if (mFrustum[i][A] * (pX + pSize) + mFrustum[i][B] * (pY - pSize) + mFrustum[i][C] * (pZ - pSize) + mFrustum[i][D] > 0)
			continue;
		if (mFrustum[i][A] * (pX - pSize) + mFrustum[i][B] * (pY + pSize) + mFrustum[i][C] * (pZ - pSize) + mFrustum[i][D] > 0)
			continue;
		if (mFrustum[i][A] * (pX + pSize) + mFrustum[i][B] * (pY + pSize) + mFrustum[i][C] * (pZ - pSize) + mFrustum[i][D] > 0)
			continue;
		if (mFrustum[i][A] * (pX - pSize) + mFrustum[i][B] * (pY - pSize) + mFrustum[i][C] * (pZ + pSize) + mFrustum[i][D] > 0)
			continue;
		if (mFrustum[i][A] * (pX + pSize) + mFrustum[i][B] * (pY - pSize) + mFrustum[i][C] * (pZ + pSize) + mFrustum[i][D] > 0)
			continue;
		if (mFrustum[i][A] * (pX - pSize) + mFrustum[i][B] * (pY + pSize) + mFrustum[i][C] * (pZ + pSize) + mFrustum[i][D] > 0)
			continue;
		if (mFrustum[i][A] * (pX + pSize) + mFrustum[i][B] * (pY + pSize) + mFrustum[i][C] * (pZ + pSize) + mFrustum[i][D] > 0)
			continue;

		return false;
	}

	return true;
}

bool Render::Render_Frustum::BoundingSphereInFrustum(const Render_BoundingSphere& pBoundingSphere, const Maths::Maths_FTransform& pTransform) const
{
	const auto& position = pTransform.GetWorldPosition();
	const auto& rotation = pTransform.GetWorldRotation();
	const auto& scale = pTransform.GetWorldScale();

	float maxScale = std::max(std::max(std::max(scale.x, scale.y), scale.z), 0.0f);
	float scaledRadius = pBoundingSphere.radius * maxScale;
	auto sphereOffset = Maths::Maths_FQuaternionExt::RotatePoint(pBoundingSphere.position, rotation) * maxScale;

	glm::vec3 worldCenter = position + sphereOffset;

	return SphereInFrustum(worldCenter.x, worldCenter.y, worldCenter.z, scaledRadius);
}

std::array<float, 4> Render::Render_Frustum::GetNearPlane() const
{
	return { mFrustum[FRONT][0], mFrustum[FRONT][1], mFrustum[FRONT][2], mFrustum[FRONT][3] };
}

std::array<float, 4> Render::Render_Frustum::GetFarPlane() const
{
	return { mFrustum[BACK][0], mFrustum[BACK][1], mFrustum[BACK][2], mFrustum[BACK][3] };
}
