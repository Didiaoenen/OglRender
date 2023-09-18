#pragma once

#include <queue>

#include <OglWindow/Window_InputManager.h>

#include <OglRenderer/Render_Camera.h>

#include "Editor_Hierarchy.h"
#include "Editor_AView.h"

namespace Editor
{
	class Editor_CameraController
	{
	public:
		Editor_CameraController
		(
			Editor_AView& p_view,
			Render::Render_Camera& pCamera,
			glm::vec3& pPosition,
			glm::quat& pRotation,
			bool p_enableFocusInputs = false
		);

		void HandleInputs(float pDeltaTime);

		void MoveToTarget(Core::Core_Actor& p_target);

		void SetSpeed(float p_speed);

		float GetSpeed() const;

		void SetPosition(const glm::vec3& pPosition);

		void SetRotation(const glm::quat& pRotation);

		const glm::vec3& GetPosition() const;

		const glm::quat& GetRotation() const;

		bool IsRightMousePressed() const;

	private:
		void HandleCameraPanning(const glm::vec2& p_mouseOffset, bool p_firstMouse);
		void HandleCameraOrbit(const glm::vec2& p_mouseOffset, bool p_firstMouse);
		void HandleCameraFPSMouse(const glm::vec2& p_mouseOffset, bool p_firstMouse);

		void HandleCameraZoom();
		void HandleCameraFPSKeyboard(float pDeltaTime);
		void UpdateMouseState();

	private:
		Window::Window_InputManager& m_inputManager;
		Window::Window_Window& mWindow;
		Editor_AView& m_view;
		Render::Render_Camera& mCamera;
		glm::vec3& mCameraPosition;
		glm::quat& mCameraRotation;

		std::queue<std::tuple<glm::vec3, glm::quat>> m_cameraDestinations;

		bool m_enableFocusInputs;

		bool m_leftMousePressed = false;
		bool m_middleMousePressed = false;
		bool m_rightMousePressed = false;

		glm::vec3 m_targetSpeed;
		glm::vec3 m_currentMovementSpeed;

		Maths::Maths_FTransform* m_orbitTarget = nullptr;
		glm::vec3 m_orbitStartOffset;
		bool m_firstMouse = true;
		double m_lastMousePosX = 0.0;
		double m_lastMousePosY = 0.0;
		glm::vec3 m_ypr;
		float m_mouseSensitivity = 0.12f;
		float m_cameraDragSpeed = 0.03f;
		float m_cameraOrbitSpeed = 0.5f;
		float m_cameraMoveSpeed = 15.0f;
		float m_focusDistance = 15.0f;
		float m_focusLerpCoefficient = 8.0f;
	};
}

