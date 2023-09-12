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
			CULL_CUSTOM = 3,
		};

		Core_CModelRenderer(Core_Actor& pOwner);

		std::string GetName() override;

		void SetModel(Render::Render_Model* pModel);

		Render::Render_Model* GetModel() const;

		void SetFrustumBehaviour(EFrustumBehaviour pBoundingMode);

		EFrustumBehaviour GetFrustumBehaviour() const;

		const Render::Render_BoundingSphere& GetCustomBoundingSphere() const;

		void SetCustomBoundingSphere(const Render::Render_BoundingSphere& pBoundingSphere);

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;

	private:
		Render::Render_Model* mModel{ nullptr };
		Tools::Tools_Event<> mModelChangedEvent;
		Render::Render_BoundingSphere mCustomBoundingSphere = { {}, 1.0f };
		EFrustumBehaviour mFrustumBehaviour = EFrustumBehaviour::CULL_MODEL;
	};
}

