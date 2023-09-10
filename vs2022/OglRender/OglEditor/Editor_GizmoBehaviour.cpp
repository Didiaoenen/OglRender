#include "Editor_EditorActions.h"
#include "Editor_EditorSettings.h"

#include "Editor_GizmoBehaviour.h"

bool Editor::Editor_GizmoBehaviour::IsSnappedBehaviourEnabled() const
{
    return false;
}

void Editor::Editor_GizmoBehaviour::StartPicking(Core::Core_Actor& p_target, const glm::vec3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction)
{
}

void Editor::Editor_GizmoBehaviour::StopPicking()
{
}

void Editor::Editor_GizmoBehaviour::ApplyOperation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize)
{
}

void Editor::Editor_GizmoBehaviour::SetCurrentMouse(const glm::vec2& p_mousePosition)
{
}

bool Editor::Editor_GizmoBehaviour::IsPicking() const
{
    return false;
}

Editor::Editor_GizmoBehaviour::EDirection Editor::Editor_GizmoBehaviour::GetDirection() const
{
    return EDirection();
}

glm::vec3 Editor::Editor_GizmoBehaviour::GetFakeDirection() const
{
    return glm::vec3();
}

glm::vec3 Editor::Editor_GizmoBehaviour::GetRealDirection(bool p_relative) const
{
    return glm::vec3();
}

glm::vec2 Editor::Editor_GizmoBehaviour::GetScreenDirection(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
    return glm::vec2();
}

void Editor::Editor_GizmoBehaviour::ApplyTranslation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
}

void Editor::Editor_GizmoBehaviour::ApplyRotation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
}

void Editor::Editor_GizmoBehaviour::ApplyScale(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
}
