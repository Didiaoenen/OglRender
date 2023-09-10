#pragma once

#include <glm/glm.hpp>

#include <OglRenderer/Render_Light.h>

#include "Core_AComponent.h"

namespace Core
{
	class Core_Actor;

	class Core_CLight : public Core_AComponent
	{
	public:
		Core_CLight(Core_Actor& p_owner);

		const Render::Render_Light& GetData() const;

		const glm::vec3& GetColor() const;

		float GetIntensity() const;

		void SetColor(const glm::vec3& p_color);

		void SetIntensity(float p_intensity);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;

	protected:
		Render::Render_Light m_data;
	};
}

