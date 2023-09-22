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

		void CacheMatrices(uint16_t pWindowWidth, uint16_t pWindowHeight, const glm::vec3& pPosition, const glm::quat& pRotation);

		void CacheProjectionMatrix(uint16_t pWindowWidth, uint16_t pWindowHeight);

		void CacheViewMatrix(const glm::vec3& pPosition, const glm::quat& pRotation);

		void CacheFrustum(const glm::mat4& pView, const glm::mat4& pProjection);

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

		void SetClearColor(const glm::vec3& pClearColor);

		void SetFrustumGeometryCulling(bool pEnable);

		void SetFrustumLightCulling(bool pEnable);

		void SetProjectionMode(EProjectionMode pProjectionMode);

	private:
		glm::mat4 CalculateProjectionMatrix(uint16_t pWindowWidth, uint16_t pWindowHeight) const;
		glm::mat4 CalculateViewMatrix(const glm::vec3& pPosition, const glm::quat& pRotation) const;

	private:
		Render_Frustum mFrustum;
		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		EProjectionMode mProjectionMode;

		float mFov;
		float mSize;
		float mNear;
		float mFar;

		glm::vec3 mClearColor;

		bool mFrustumGeometryCulling;
		bool mFrustumLightCulling;
	};
}

