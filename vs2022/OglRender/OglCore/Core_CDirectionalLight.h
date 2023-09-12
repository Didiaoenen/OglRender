#pragma once

#include "Core_CLight.h"

namespace Core
{
	class Core_Actor;

	class Core_CDirectionalLight : public Core_CLight
	{
	public:
		Core_CDirectionalLight(Core_Actor& pOwner);

		std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;
	};
}

