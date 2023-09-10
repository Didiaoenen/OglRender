#pragma once

#include <OglRenderer/Render_Camera.h>

#include "Core_AComponent.h"

namespace Core
{
	class Core_Actor;

	class Core_CCamera : public Core_AComponent
	{
	public:
		Core_CCamera(Core_Actor& p_owner);

		~Core_CCamera() = default;

		std::string GetName() override;

		void SetFov(float p_value);

		void SetSize(float p_value);

		void SetNear(float p_value);

		void SetFar(float p_value);

		void SetClearColor(const glm::vec3& p_clearColor);

		void SetFrustumGeometryCulling(bool p_enable);

		void SetFrustumLightCulling(bool p_enable);

		void SetProjectionMode(Render::EProjectionMode p_projectionMode);

		float GetFov() const;

		float GetSize() const;

		float GetNear() const;

		float GetFar() const;

		const glm::vec3& GetClearColor() const;

		bool HasFrustumGeometryCulling() const;

		bool HasFrustumLightCulling() const;

		Render::EProjectionMode GetProjectionMode() const;

		Render::Render_Camera& GetCamera();

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;

	private:
		Render::Render_Camera m_camera;
	};
}

