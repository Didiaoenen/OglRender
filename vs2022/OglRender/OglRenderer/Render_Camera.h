#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>

#include <OglTools/Tools_ReferenceOrValue.h>

#include "Render_Frustum.h"
#include "Render_EProjectionMode.h"

namespace Render
{
	class Render_Camera
	{
	public:
		Render_Camera();

		void CacheMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const glm::vec3& p_position, const glm::quat& p_rotation);

		void CacheProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight);

		void CacheViewMatrix(const glm::vec3& p_position, const glm::quat& p_rotation);

		void CacheFrustum(const glm::mat4& p_view, const glm::mat4& p_projection);

		float GetFov() const;

		float GetSize() const;

		float GetNear() const;

		float GetFar() const;

		const glm::vec3& GetClearColor() const;

		const glm::mat4& GetProjectionMatrix() const;

		const glm::mat4& GetViewMatrix() const;

		const Render_Frustum& GetFrustum() const;

		bool HasFrustumGeometryCulling() const;

		bool HasFrustumLightCulling() const;

		EProjectionMode GetProjectionMode() const;

		void SetFov(float pValue);

		void SetSize(float pValue);

		void SetNear(float pValue);

		void SetFar(float pValue);

		void SetClearColor(const glm::vec3& p_clearColor);

		void SetFrustumGeometryCulling(bool p_enable);

		void SetFrustumLightCulling(bool p_enable);

		void SetProjectionMode(EProjectionMode p_projectionMode);

	private:
		glm::mat4 CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const;
		glm::mat4 CalculateViewMatrix(const glm::vec3& p_position, const glm::quat& p_rotation) const;

	private:
		Render_Frustum m_frustum;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		EProjectionMode m_projectionMode;

		float m_fov;
		float m_size;
		float m_near;
		float m_far;

		glm::vec3 m_clearColor;

		bool m_frustumGeometryCulling;
		bool m_frustumLightCulling;
	};
}

