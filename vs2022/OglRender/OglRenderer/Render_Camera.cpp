#include "Render_Camera.h"

Render::Render_Camera::Render_Camera() :
	m_projectionMode(EProjectionMode::PERSPECTIVE),
	m_fov(45.0f),
	m_size(5.0f),
	m_near(0.1f),
	m_far(100.f),
	m_clearColor(0.f, 0.f, 0.f),
	m_frustumGeometryCulling(false),
	m_frustumLightCulling(false)
{
}

void Render::Render_Camera::CacheMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const glm::vec3& p_position, const glm::quat& pRotation)
{
	CacheProjectionMatrix(p_windowWidth, p_windowHeight);
	CacheViewMatrix(p_position, pRotation);
	CacheFrustum(m_viewMatrix, m_projectionMatrix);
}

void Render::Render_Camera::CacheProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight)
{
	m_projectionMatrix = CalculateProjectionMatrix(p_windowWidth, p_windowHeight);
}

void Render::Render_Camera::CacheViewMatrix(const glm::vec3& p_position, const glm::quat& pRotation)
{
	m_viewMatrix = CalculateViewMatrix(p_position, pRotation);
}

void Render::Render_Camera::CacheFrustum(const glm::mat4& p_view, const glm::mat4& p_projection)
{
	m_frustum.CalculateFrustum(p_projection * p_view);
}

float Render::Render_Camera::GetFov() const
{
	return m_fov;
}

float Render::Render_Camera::GetSize() const
{
	return m_size;
}

float Render::Render_Camera::GetNear() const
{
	return m_near;
}

float Render::Render_Camera::GetFar() const
{
	return m_far;
}

const glm::vec3& Render::Render_Camera::GetClearColor() const
{
	return m_clearColor;
}

const glm::mat4& Render::Render_Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const glm::mat4& Render::Render_Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const Render::Render_Frustum& Render::Render_Camera::GetFrustum() const
{
	return m_frustum;
}

bool Render::Render_Camera::HasFrustumGeometryCulling() const
{
	return m_frustumGeometryCulling;
}

bool Render::Render_Camera::HasFrustumLightCulling() const
{
	return m_frustumLightCulling;
}

Render::EProjectionMode Render::Render_Camera::GetProjectionMode() const
{
	return m_projectionMode;
}

void Render::Render_Camera::SetFov(float pValue)
{
	m_fov = pValue;
}

void Render::Render_Camera::SetSize(float pValue)
{
	m_size = pValue;
}

void Render::Render_Camera::SetNear(float pValue)
{
	m_near = pValue;
}

void Render::Render_Camera::SetFar(float pValue)
{
	m_far = pValue;
}

void Render::Render_Camera::SetClearColor(const glm::vec3& pClearColor)
{
	m_clearColor = pClearColor;
}

void Render::Render_Camera::SetFrustumGeometryCulling(bool pEnable)
{
	m_frustumGeometryCulling = pEnable;
}

void Render::Render_Camera::SetFrustumLightCulling(bool pEnable)
{
	m_frustumLightCulling = pEnable;
}

void Render::Render_Camera::SetProjectionMode(EProjectionMode p_projectionMode)
{
	m_projectionMode = p_projectionMode;
}

glm::mat4 Render::Render_Camera::CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const
{
	const auto ratio = p_windowWidth / static_cast<float>(p_windowHeight);

	switch (m_projectionMode)
	{
	case EProjectionMode::ORTHOGRAPHIC:
		return glm::ortho(m_size, ratio, m_near, m_far);

	case EProjectionMode::PERSPECTIVE:
		return glm::perspective(m_fov, ratio, m_near, m_far);

	default:
		return glm::identity<glm::mat4>();
	}
}

glm::mat4 Render::Render_Camera::CalculateViewMatrix(const glm::vec3& p_position, const glm::quat& pRotation) const
{
	const auto& up = pRotation * glm::vec3(0.f, 1.f, 0.f);
	const auto& forward = pRotation * glm::vec3(0.f, 0.f, -1.f);
	return glm::lookAt(p_position, p_position + forward, up);
}

