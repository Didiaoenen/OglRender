#include "Render_Camera.h"

Render::Render_Camera::Render_Camera() :
	mProjectionMode(EProjectionMode::PERSPECTIVE),
	mFov(45.0f),
	mSize(5.0f),
	mNear(0.1f),
	mFar(100.f),
	mClearColor(0.f, 0.f, 0.f),
	mFrustumGeometryCulling(false),
	mFrustumLightCulling(false)
{
}

void Render::Render_Camera::CacheMatrices(uint16_t pWindowWidth, uint16_t pWindowHeight, const glm::vec3& pPosition, const glm::quat& pRotation)
{
	CacheProjectionMatrix(pWindowWidth, pWindowHeight);
	CacheViewMatrix(pPosition, pRotation);
	CacheFrustum(mViewMatrix, mProjectionMatrix);
}

void Render::Render_Camera::CacheProjectionMatrix(uint16_t pWindowWidth, uint16_t pWindowHeight)
{
	mProjectionMatrix = CalculateProjectionMatrix(pWindowWidth, pWindowHeight);
}

void Render::Render_Camera::CacheViewMatrix(const glm::vec3& pPosition, const glm::quat& pRotation)
{
	mViewMatrix = CalculateViewMatrix(pPosition, pRotation);
}

void Render::Render_Camera::CacheFrustum(const glm::mat4& pView, const glm::mat4& pProjection)
{
	mFrustum.CalculateFrustum(pProjection * pView);
}

float Render::Render_Camera::GetFov() const
{
	return mFov;
}

float Render::Render_Camera::GetSize() const
{
	return mSize;
}

float Render::Render_Camera::GetNear() const
{
	return mNear;
}

float Render::Render_Camera::GetFar() const
{
	return mFar;
}

const glm::vec3& Render::Render_Camera::GetClearColor() const
{
	return mClearColor;
}

const glm::mat4& Render::Render_Camera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

const glm::mat4& Render::Render_Camera::GetViewMatrix() const
{
	return mViewMatrix;
}

const Render::Render_Frustum& Render::Render_Camera::GetFrustum() const
{
	return mFrustum;
}

bool Render::Render_Camera::HasFrustumGeometryCulling() const
{
	return mFrustumGeometryCulling;
}

bool Render::Render_Camera::HasFrustumLightCulling() const
{
	return mFrustumLightCulling;
}

Render::EProjectionMode Render::Render_Camera::GetProjectionMode() const
{
	return mProjectionMode;
}

void Render::Render_Camera::SetFov(float pValue)
{
	mFov = pValue;
}

void Render::Render_Camera::SetSize(float pValue)
{
	mSize = pValue;
}

void Render::Render_Camera::SetNear(float pValue)
{
	mNear = pValue;
}

void Render::Render_Camera::SetFar(float pValue)
{
	mFar = pValue;
}

void Render::Render_Camera::SetClearColor(const glm::vec3& pClearColor)
{
	mClearColor = pClearColor;
}

void Render::Render_Camera::SetFrustumGeometryCulling(bool pEnable)
{
	mFrustumGeometryCulling = pEnable;
}

void Render::Render_Camera::SetFrustumLightCulling(bool pEnable)
{
	mFrustumLightCulling = pEnable;
}

void Render::Render_Camera::SetProjectionMode(EProjectionMode pProjectionMode)
{
	mProjectionMode = pProjectionMode;
}

glm::mat4 Render::Render_Camera::CalculateProjectionMatrix(uint16_t pWindowWidth, uint16_t pWindowHeight) const
{
	const auto ratio = pWindowWidth / static_cast<float>(pWindowHeight);

	switch (mProjectionMode)
	{
	case EProjectionMode::ORTHOGRAPHIC:
		return glm::ortho(mSize, ratio, mNear, mFar);

	case EProjectionMode::PERSPECTIVE:
		return glm::perspective(mFov, ratio, mNear, mFar);

	default:
		return glm::identity<glm::mat4>();
	}
}

glm::mat4 Render::Render_Camera::CalculateViewMatrix(const glm::vec3& pPosition, const glm::quat& pRotation) const
{
	const auto& up = pRotation * glm::vec3(0.f, 1.f, 0.f);
	const auto& forward = pRotation * glm::vec3(0.f, 0.f, -1.f);
	return glm::lookAt(pPosition, pPosition + forward, up);
}

