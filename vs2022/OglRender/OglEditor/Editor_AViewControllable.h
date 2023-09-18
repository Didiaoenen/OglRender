#pragma once

#include "Editor_AView.h"
#include "Editor_CameraController.h"

namespace Editor
{
	class Editor_AViewControllable : public Editor_AView
	{
	public:
		Editor_AViewControllable
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings,
			bool p_enableFocusInputs = false
		);

		virtual void Update(float pDeltaTime) override;

		Editor_CameraController& GetCameraController();

	protected:
		Editor_CameraController mCameraController;
	};
}

