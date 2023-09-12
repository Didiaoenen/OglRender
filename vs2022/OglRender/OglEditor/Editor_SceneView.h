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
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings
		);

		virtual void Update(float pDeltaTime) override;

		virtual void _Render_Impl() override;

		void RenderScene(uint8_t p_defaultRenderState);

		void RenderSceneForActorPicking();

		void HandleActorPicking();

	private:
		Core::Core_SceneManager& m_sceneManager;
		Render::Render_Framebuffer m_actorPickingFramebuffer;
		Editor_GizmoBehaviour m_gizmoOperations;
		EGizmoOperation m_currentOperation = EGizmoOperation::TRANSLATE;

		std::optional<std::reference_wrapper<Core::Core_Actor>> m_highlightedActor;
		std::optional<Editor_GizmoBehaviour::EDirection> m_highlightedGizmoDirection;
	};
}

