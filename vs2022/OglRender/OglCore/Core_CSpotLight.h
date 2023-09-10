#pragma once

#include "Core_CLight.h"

namespace Core
{
	class Core_Actor;

	class Core_CSpotLight : public Core_CLight
	{
	public:
		Core_CSpotLight(Core_Actor& p_owner);

		std::string GetName() override;

		float GetConstant() const;

		float GetLinear() const;

		float GetQuadratic() const;

		float GetCutoff() const;

		float GetOuterCutoff() const;

		void SetConstant(float p_constant);

		void SetLinear(float p_linear);

		void SetQuadratic(float p_quadratic);

		void SetCutoff(float p_cutoff);

		void SetOuterCutoff(float p_outerCutoff);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;
	};
}

