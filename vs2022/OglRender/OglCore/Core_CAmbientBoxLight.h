#pragma once

#include "Core_CLight.h"

namespace Core
{
	class Core_Actor;

	class Core_CAmbientBoxLight : public Core_CLight
	{
	public:
		Core_CAmbientBoxLight(Core_Actor& p_owner);

		std::string GetName() override;

		glm::vec3 GetSize() const;

		void SetSize(const glm::vec3& p_size);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;
	};
}

