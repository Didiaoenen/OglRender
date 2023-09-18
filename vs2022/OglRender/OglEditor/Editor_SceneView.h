#pragma once

#include "Editor_GizmoBehaviour.h"
#include "Editor_AViewControllable.h"

namespace Editor
{
	class Editor_SceneView : public Editor_AViewControllable
	{
	public:
		Editor_SceneView
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		virtual void Update(float pDeltaTime) override;

		virtual void _Render_Impl() override;

		void RenderScene(uint8_t pDefaultRenderState);

		void RenderSceneForActorPicking();

		void HandleActorPicking();

	private:
		Core::Core_SceneManager& mSceneManager;
		Render::Render_Framebuffer mActorPickingFramebuffer;
		Editor_GizmoBehaviour mGizmoOperations;
		EGizmoOperation mCurrentOperation = EGizmoOperation::TRANSLATE;

		std::optional<std::reference_wrapper<Core::Core_Actor>> mHighlightedActor;
		std::optional<Editor_GizmoBehaviour::EDirection> mHighlightedGizmoDirection;
	};
}

