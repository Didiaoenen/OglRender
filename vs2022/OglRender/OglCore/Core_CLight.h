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
		Core_CLight(Core_Actor& pOwner);

		const Render::Render_Light& GetData() const;

		const glm::vec3& GetColor() const;

		float GetIntensity() const;

		void SetColor(const glm::vec3& pColor);

		void SetIntensity(float pIntensity);

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;

	protected:
		Render::Render_Light mData;
	};
}

