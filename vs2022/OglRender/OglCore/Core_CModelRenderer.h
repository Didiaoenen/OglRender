#pragma once

#include <OglRenderer/Render_Vertex.h>
#include <OglRenderer/Render_Model.h>

#include "Core_AComponent.h"

namespace Core
{
	class Core_Actor;

	class Core_CModelRenderer : public Core_AComponent
	{
	public:
		enum class EFrustumBehaviour
		{
			DISABLED = 0,
			CULL_MODEL = 1,
			CULL_MESHES = 2,
			CULL_CUSTOM = 3
		};

		Core_CModelRenderer(Core_Actor& p_owner);

		std::string GetName() override;

		void SetModel(Render::Render_Model* p_model);

		Render::Render_Model* GetModel() const;

		void SetFrustumBehaviour(EFrustumBehaviour p_boundingMode);

		EFrustumBehaviour GetFrustumBehaviour() const;

		const Render::Render_BoundingSphere& GetCustomBoundingSphere() const;

		void SetCustomBoundingSphere(const Render::Render_BoundingSphere& p_boundingSphere);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;

	private:
		Render::Render_Model* m_model = nullptr;
		Tools::Tools_Event<> m_modelChangedEvent;
		Render::Render_BoundingSphere m_customBoundingSphere = { {}, 1.0f };
		EFrustumBehaviour m_frustumBehaviour = EFrustumBehaviour::CULL_MODEL;
	};
}

