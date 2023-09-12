#pragma once

#include "Core_CLight.h"

namespace Core
{
	class Core_Actor;

	class Core_CAmbientSphereLight : public Core_CLight
	{
	public:
		Core_CAmbientSphereLight(Core_Actor& pOwner);

		std::string GetName() override;

		float GetRadius() const;

		void SetRadius(float pRadius);

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;
		
		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;
	};
}

