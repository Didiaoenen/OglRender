#pragma once

#include "Core_CLight.h"

namespace Core
{
	class Core_Actor;

	class Core_CSpotLight : public Core_CLight
	{
	public:
		Core_CSpotLight(Core_Actor& pOwner);

		std::string GetName() override;

		float GetConstant() const;

		float GetLinear() const;

		float GetQuadratic() const;

		float GetCutoff() const;

		float GetOuterCutoff() const;

		void SetConstant(float pConstant);

		void SetLinear(float pLinear);

		void SetQuadratic(float pQuadratic);

		void SetCutoff(float pCutoff);

		void SetOuterCutoff(float pOuterCutoff);

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;
	};
}

