#include <OglCore/Core_CAmbientBoxLight.h>
#include <OglCore/Core_CAmbientSphereLight.h>

#include "OglUI/imgui.h"

#include "Editor_EditorActions.h"
#include "Editor_CameraController.h"

Editor::Editor_CameraController::Editor_CameraController(Editor_AView& p_view, Render::Render_Camera& p_camera, glm::vec3& p_position, glm::quat& p_rotation, bool p_enableFocusInputs) :
	m_inputManager(*EDITOR_CONTEXT(inputManager)),
	m_window(*EDITOR_CONTEXT(window)),
	m_view(p_view),
	m_camera(p_camera),
	m_cameraPosition(p_position),
	m_cameraRotation(p_rotation),
	m_enableFocusInputs(p_enableFocusInputs)
{
}

void Editor::Editor_CameraController::HandleInputs(float p_deltaTime)
{
}

void Editor::Editor_CameraController::MoveToTarget(Core::Core_Actor& p_target)
{
}

void Editor::Editor_CameraController::SetSpeed(float p_speed)
{
}

float Editor::Editor_CameraController::GetSpeed() const
{
	return 0.0f;
}

void Editor::Editor_CameraController::SetPosition(const glm::vec3& p_position)
{
}

void Editor::Editor_CameraController::SetRotation(const glm::quat& p_rotation)
{
}

const glm::vec3& Editor::Editor_CameraController::GetPosition() const
{
	return m_cameraPosition;
}

const glm::quat& Editor::Editor_CameraController::GetRotation() const
{
	return m_cameraRotation;
}

bool Editor::Editor_CameraController::IsRightMousePressed() const
{
	return false;
}

void Editor::Editor_CameraController::HandleCameraPanning(const glm::vec2& p_mouseOffset, bool p_firstMouse)
{
}

void Editor::Editor_CameraController::HandleCameraOrbit(const glm::vec2& p_mouseOffset, bool p_firstMouse)
{
}

void Editor::Editor_CameraController::HandleCameraFPSMouse(const glm::vec2& p_mouseOffset, bool p_firstMouse)
{
}

void Editor::Editor_CameraController::HandleCameraZoom()
{
}

void Editor::Editor_CameraController::HandleCameraFPSKeyboard(float p_deltaTime)
{
}

void Editor::Editor_CameraController::UpdateMouseState()
{
}
