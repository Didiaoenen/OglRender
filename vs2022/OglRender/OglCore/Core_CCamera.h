#pragma once

#include <OglRenderer/Render_Camera.h>

#include "Core_AComponent.h"

namespace Core
{
	class Core_Actor;

	class Core_CCamera : public Core_AComponent
	{
	public:
		Core_CCamera(Core_Actor& pOwner);

		~Core_CCamera() = default;

		std::string GetName() override;

		void SetFov(float pValue);

		void SetSize(float pValue);

		void SetNear(float pValue);

		void SetFar(float pValue);

		void SetClearColor(const glm::vec3& pClearColor);

		void SetFrustumGeometryCulling(bool pEnable);

		void SetFrustumLightCulling(bool pEnable);

		void SetProjectionMode(Render::EProjectionMode pProjectionMode);

		float GetFov() const;

		float GetSize() const;

		float GetNear() const;

		float GetFar() const;

		const glm::vec3& GetClearColor() const;

		bool HasFrustumGeometryCulling() const;

		bool HasFrustumLightCulling() const;

		Render::EProjectionMode GetProjectionMode() const;

		Render::Render_Camera& GetCamera();

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;

	private:
		Render::Render_Camera mCamera;
	};
}

