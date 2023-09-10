#pragma once

#include <OglRenderer/Render_Light.h>

#include "Core_CLight.h"

namespace Core
{
	class Core_Actor;

	class Core_CPointLight : public Core_CLight
	{
	public:
		Core_CPointLight(Core_Actor& p_owner);

		std::string GetName() override;

		float GetConstant() const;

		float GetLinear() const;

		float GetQuadratic() const;

		void SetConstant(float p_constant);

		void SetLinear(float p_linear);

		void SetQuadratic(float p_quadratic);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;
	};
}

