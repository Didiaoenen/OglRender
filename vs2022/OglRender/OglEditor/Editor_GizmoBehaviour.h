#pragma once

#include <OglMaths/Maths_FTransform.h>

#include <OglCore/Core_Actor.h>

namespace Editor
{
	enum class EGizmoOperation
	{
		TRANSLATE,
		ROTATE,
		SCALE,
	};

	class Editor_GizmoBehaviour
	{
	public:
		enum class EDirection
		{
			X,
			Y,
			Z
		};

		bool IsSnappedBehaviourEnabled() const;

		void StartPicking(Core::Core_Actor& p_target, const glm::vec3& pCameraPosition, EGizmoOperation p_operation, EDirection p_direction);

		void StopPicking();

		void ApplyOperation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize);

		void SetCurrentMouse(const glm::vec2& p_mousePosition);

		bool IsPicking() const;

		EDirection GetDirection() const;

	private:
		glm::vec3 GetFakeDirection() const;

		glm::vec3 GetRealDirection(bool p_relative = false) const;

		glm::vec2 GetScreenDirection(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const;

		void ApplyTranslation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const;

		void ApplyRotation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const;

		void ApplyScale(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const;

	private:
		bool m_firstMouse = true;
		float m_distanceToActor = 0.0f;
		Core::Core_Actor* m_target = nullptr;
		EGizmoOperation mCurrentOperation;
		EDirection m_direction;
		Maths::Maths_FTransform m_originalTransform;
		glm::vec2 m_originMouse;
		glm::vec2 m_currentMouse;
		glm::vec2 m_screenDirection;
	};
}

