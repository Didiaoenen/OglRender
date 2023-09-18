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
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		virtual void Update(float pDeltaTime) override;

		virtual void _Render_Impl() override;

		bool HasCamera() const;

		std::optional<Render::Render_Frustum> GetActiveFrustum() const;

	private:
		Core::Core_SceneManager& mSceneManager;
		bool mHasCamera = false;
	};
}


