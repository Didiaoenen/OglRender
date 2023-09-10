#include "Editor_AViewControllable.h"

Editor::Editor_AViewControllable::Editor_AViewControllable(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings, bool p_enableFocusInputs) :
	Editor_AView(p_title, p_opened, p_windowSettings), m_cameraController(*this, m_camera, m_cameraPosition, m_cameraRotation, p_enableFocusInputs)
{
}

void Editor::Editor_AViewControllable::Update(float pDeltaTime)
{
}

Editor::Editor_CameraController& Editor::Editor_AViewControllable::GetCameraController()
{
	return m_cameraController;
}
