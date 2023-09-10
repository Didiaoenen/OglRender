#pragma once

#include "Core_CLight.h"

namespace Core
{
	class Core_Actor;

	class Core_CAmbientSphereLight : public Core_CLight
	{
	public:
		Core_CAmbientSphereLight(Core_Actor& p_owner);

		std::string GetName() override;

		float GetRadius() const;

		void SetRadius(float p_radius);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;
	};
}

