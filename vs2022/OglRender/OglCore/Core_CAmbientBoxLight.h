#pragma once

#include "Core_CLight.h"

namespace Core
{
	class Core_Actor;

	class Core_CAmbientBoxLight : public Core_CLight
	{
	public:
		Core_CAmbientBoxLight(Core_Actor& pOwner);

		std::string GetName() override;

		glm::vec3 GetSize() const;

		void SetSize(const glm::vec3& pSize);

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;
	};
}

