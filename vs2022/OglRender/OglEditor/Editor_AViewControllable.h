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
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings,
			bool p_enableFocusInputs = false
		);

		virtual void Update(float pDeltaTime) override;

		Editor_CameraController& GetCameraController();

	protected:
		Editor_CameraController m_cameraController;
	};
}

