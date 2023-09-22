#pragma once

#include <queue>
#include <glm/gtx/compatibility.hpp>

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
			Editor_AView& pView,
			Render::Render_Camera& pCamera,
			glm::vec3& pPosition,
			glm::quat& pRotation,
			bool pEnableFocusInputs = false
		);

		void HandleInputs(float pDeltaTime);

		void MoveToTarget(Core::Core_Actor& pTarget);

		void SetSpeed(float pSpeed);

		float GetSpeed() const;

		void SetPosition(const glm::vec3& pPosition);

		void SetRotation(const glm::quat& pRotation);

		const glm::vec3& GetPosition() const;

		const glm::quat& GetRotation() const;

		bool IsRightMousePressed() const;

	private:
		void HandleCameraPanning(const glm::vec2& pMouseOffset, bool pFirstMouse);
		void HandleCameraOrbit(const glm::vec2& pMouseOffset, bool pFirstMouse);
		void HandleCameraFPSMouse(const glm::vec2& pMouseOffset, bool pFirstMouse);

		void HandleCameraZoom();
		void HandleCameraFPSKeyboard(float pDeltaTime);
		void UpdateMouseState();

	private:
		Window::Window_InputManager& mInputManager;
		Window::Window_Window& mWindow;
		Editor_AView& mView;
		Render::Render_Camera& mCamera;
		glm::vec3& mCameraPosition;
		glm::quat& mCameraRotation;

		std::queue<std::tuple<glm::vec3, glm::quat>> mCameraDestinations;

		bool mEnableFocusInputs;

		bool mLeftMousePressed = false;
		bool mMiddleMousePressed = false;
		bool mRightMousePressed = false;

		glm::vec3 mTargetSpeed;
		glm::vec3 mCurrentMovementSpeed;

		Maths::Maths_FTransform* mOrbitTarget = nullptr;
		glm::vec3 mOrbitStartOffset;
		bool mFirstMouse = true;
		double mLastMousePosX = 0.0;
		double mLastMousePosY = 0.0;
		glm::vec3 mYPR;
		float mMouseSensitivity = 0.12f;
		float mCameraDragSpeed = 0.03f;
		float mCameraOrbitSpeed = 0.5f;
		float mCameraMoveSpeed = 15.0f;
		float mFocusDistance = 15.0f;
		float mFocusLerpCoefficient = 8.0f;
	};
}

