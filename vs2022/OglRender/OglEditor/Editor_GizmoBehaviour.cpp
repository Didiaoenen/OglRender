#include "Editor_EditorActions.h"
#include "Editor_EditorSettings.h"

#include "Editor_GizmoBehaviour.h"

float SnapValue(float p_value, float p_step)
{
    return p_value - std::fmod(p_value, p_step);
}

bool Editor::Editor_GizmoBehaviour::IsSnappedBehaviourEnabled() const
{
    const auto& inputManager = EDITOR_CONTEXT(mInputManager);
    return inputManager->GetKeyState(Window::EKey::KEY_LEFT_CONTROL) == Window::EKeyState::KEY_DOWN || inputManager->GetKeyState(Window::EKey::KEY_RIGHT_CONTROL) == Window::EKeyState::KEY_DOWN;
}

void Editor::Editor_GizmoBehaviour::StartPicking(Core::Core_Actor& pTarget, const glm::vec3& pCameraPosition, EGizmoOperation pOperation, EDirection p_direction)
{
    mTarget = &pTarget;
    mFirstMouse = true;
    m_originalTransform = pTarget.transform.GetFTransform();
    m_distanceToActor = glm::distance(pCameraPosition, mTarget->transform.GetWorldPosition());
    mCurrentOperation = pOperation;
    m_direction = p_direction;
}

void Editor::Editor_GizmoBehaviour::StopPicking()
{
    mTarget = nullptr;
}

void Editor::Editor_GizmoBehaviour::ApplyOperation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize)
{
    switch (mCurrentOperation)
    {
    case EGizmoOperation::TRANSLATE:
        ApplyTranslation(p_viewMatrix, p_projectionMatrix, p_viewSize);
        break;

    case EGizmoOperation::ROTATE:
        ApplyRotation(p_viewMatrix, p_projectionMatrix, p_viewSize);
        break;

    case EGizmoOperation::SCALE:
        ApplyScale(p_viewMatrix, p_projectionMatrix, p_viewSize);
        break;
    }
}

void Editor::Editor_GizmoBehaviour::SetCurrentMouse(const glm::vec2& p_mousePosition)
{
    if (mFirstMouse)
    {
        m_currentMouse = m_originMouse = p_mousePosition;
        mFirstMouse = false;
    }
    else
    {
        m_currentMouse = p_mousePosition;
    }
}

bool Editor::Editor_GizmoBehaviour::IsPicking() const
{
    return mTarget;
}

Editor::Editor_GizmoBehaviour::EDirection Editor::Editor_GizmoBehaviour::GetDirection() const
{
    return m_direction;
}

glm::vec3 Editor::Editor_GizmoBehaviour::GetFakeDirection() const
{
    auto result = glm::vec3();

    switch (m_direction)
    {
    case Editor_GizmoBehaviour::EDirection::X:
        result = glm::vec3(1.f, 0.f, 0.f);
        break;
    case Editor_GizmoBehaviour::EDirection::Y:
        result = glm::vec3(0.f, 1.f, 0.f);
        break;
    case Editor_GizmoBehaviour::EDirection::Z:
        result = glm::vec3(0.f, 0.f, 1.f);
        break;
    }

    return result;
}

glm::vec3 Editor::Editor_GizmoBehaviour::GetRealDirection(bool p_relative) const
{
    auto result = glm::vec3();

    switch (m_direction)
    {
    case Editor_GizmoBehaviour::EDirection::X:
        result = p_relative ? m_originalTransform.GetWorldRight() : m_originalTransform.GetLocalRight();
        break;
    case Editor_GizmoBehaviour::EDirection::Y:
        result = p_relative ? m_originalTransform.GetWorldUp() : m_originalTransform.GetLocalUp();
        break;
    case Editor_GizmoBehaviour::EDirection::Z:
        result = p_relative ? m_originalTransform.GetWorldForward() : m_originalTransform.GetLocalForward();
        break;
    }

    return result;
}

glm::vec2 Editor::Editor_GizmoBehaviour::GetScreenDirection(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
    auto start = m_originalTransform.GetWorldPosition();
    auto end = m_originalTransform.GetWorldPosition() + GetRealDirection(true) * 0.01f;

    auto start2D = glm::vec2();
    {
        auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * glm::vec4{ start.x, start.y, start.z, 1.0f });
        auto ndcSpacePos = glm::vec3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
        auto windowSpacePos = ((glm::vec2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
        windowSpacePos.x *= p_viewSize.x;
        windowSpacePos.y *= p_viewSize.y;
        start2D = windowSpacePos;
    }

    auto end2D = glm::vec2();
    {
        auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * glm::vec4{ end.x, end.y, end.z, 1.0f });
        auto ndcSpacePos = glm::vec3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
        auto windowSpacePos = ((glm::vec2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
        windowSpacePos.x *= p_viewSize.x;
        windowSpacePos.y *= p_viewSize.y;
        end2D = windowSpacePos;
    }

    auto result = end2D - start2D;

    result.y *= -1;

    return glm::normalize(result);
}

void Editor::Editor_GizmoBehaviour::ApplyTranslation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
    auto unitsPerPixel = 0.001f * m_distanceToActor;
    auto originPosition = m_originalTransform.GetWorldPosition();

    auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

    auto totalDisplacement = m_currentMouse - m_originMouse;
    auto translationCoefficient = glm::dot(totalDisplacement, screenDirection) * unitsPerPixel;

    if (IsSnappedBehaviourEnabled())
    {
        translationCoefficient = SnapValue(translationCoefficient, Editor_EditorSettings::TranslationSnapUnit);
    }

    mTarget->transform.SetWorldPosition(originPosition + GetRealDirection(true) * translationCoefficient);
}

void Editor::Editor_GizmoBehaviour::ApplyRotation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
    auto unitsPerPixel = 0.2f;
    auto originRotation = m_originalTransform.GetWorldRotation();

    auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);
    screenDirection = glm::vec2(-screenDirection.y, screenDirection.x);

    auto totalDisplacement = m_currentMouse - m_originMouse;
    auto rotationCoefficient = glm::dot(totalDisplacement, screenDirection) * unitsPerPixel;

    if (IsSnappedBehaviourEnabled())
    {
        rotationCoefficient = SnapValue(rotationCoefficient, Editor_EditorSettings::RotationSnapUnit);
    }

    auto rotationToApply = glm::quat(glm::vec3(GetFakeDirection() * rotationCoefficient));
    mTarget->transform.SetWorldRotation(originRotation * rotationToApply);
}

void Editor::Editor_GizmoBehaviour::ApplyScale(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const
{
    auto unitsPerPixel = 0.01f;
    auto originScale = m_originalTransform.GetWorldScale();

    auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

    auto totalDisplacement = m_currentMouse - m_originMouse;
    auto scaleCoefficient = glm::dot(totalDisplacement, screenDirection) * unitsPerPixel;

    if (IsSnappedBehaviourEnabled())
    {
        scaleCoefficient = SnapValue(scaleCoefficient, Editor_EditorSettings::ScalingSnapUnit);
    }

    auto newScale = originScale + GetFakeDirection() * scaleCoefficient;

    newScale.x = std::max(newScale.x, 0.0001f);
    newScale.y = std::max(newScale.y, 0.0001f);
    newScale.z = std::max(newScale.z, 0.0001f);

    mTarget->transform.SetWorldScale(newScale);
}
