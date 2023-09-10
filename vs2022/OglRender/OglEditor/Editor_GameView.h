#pragma once

#include <OglCore/Core_SceneManager.h>

#include "Editor_AView.h"

namespace Editor 
{
	class Editor_EditorRenderer;

	class Editor_GameView : public Editor_AView
	{
	public:
		Editor_GameView
		(
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings
		);

		virtual void Update(float p_deltaTime) override;

		virtual void _Render_Impl() override;

		bool HasCamera() const;

		std::optional<Render::Render_Frustum> GetActiveFrustum() const;

	private:
		Core::Core_SceneManager& m_sceneManager;
		bool m_hasCamera = false;
	};
}


