#include <OglCore/Core_CAmbientBoxLight.h>
#include <OglCore/Core_CAmbientSphereLight.h>

#include <OglMaths/Maths_GlmExt.h>

#include "OglUI/imgui.h"

#include "Editor_EditorActions.h"
#include "Editor_CameraController.h"

Editor::Editor_CameraController::Editor_CameraController(Editor_AView& pView, Render::Render_Camera& pCamera, glm::vec3& pPosition, glm::quat& pRotation, bool pEnableFocusInputs) :
	mInputManager(*EDITOR_CONTEXT(mInputManager)),
	mWindow(*EDITOR_CONTEXT(mWindow)),
	mView(pView),
	mCamera(pCamera),
	mCameraPosition(pPosition),
	mCameraRotation(pRotation),
	mEnableFocusInputs(pEnableFocusInputs)
{
	mCamera.SetFov(60.0f);
}

float GetActorFocusDist(Core::Core_Actor& pActor)
{
	float distance = 4.0f;

	if (pActor.IsActive())
	{
		if (auto modelRenderer = pActor.GetComponent<Core::Core_CModelRenderer>())
		{
			const bool hasCustomBoundingSphere = modelRenderer->GetFrustumBehaviour() == Core::Core_CModelRenderer::EFrustumBehaviour::CULL_CUSTOM;
			const bool hasModel = modelRenderer->GetModel();
			const auto boundingSphere = hasCustomBoundingSphere ? &modelRenderer->GetCustomBoundingSphere() : hasModel ? &modelRenderer->GetModel()->GetBoundingSphere() : nullptr;
			const auto& actorPosition = pActor.transform.GetWorldPosition();
			const auto& actorScale = pActor.transform.GetWorldScale();
			const auto scaleFactor = std::max(std::max(actorScale.x, actorScale.y), actorScale.z);

			distance = std::max(distance, boundingSphere ? (boundingSphere->radius + glm::length(boundingSphere->position)) * scaleFactor * 2.0f : 10.0f);
		}

		for (auto child : pActor.GetChildren())
		{
			distance = std::max(distance, GetActorFocusDist(*child));
		}
	}

	return distance;
}

void Editor::Editor_CameraController::HandleInputs(float pDeltaTime)
{
	if (mView.IsHovered())
	{
		UpdateMouseState();

		ImGui::GetIO().DisableMouseUpdate = mRightMousePressed || mMiddleMousePressed;

		if (!ImGui::IsAnyItemActive() && mEnableFocusInputs)
		{
			if (EDITOR_EXEC(IsAnyActorSelected()))
			{
				auto targetPos = EDITOR_EXEC(GetSelectedActor()).transform.GetWorldPosition();

				float dist = GetActorFocusDist(EDITOR_EXEC(GetSelectedActor()));

				if (mInputManager.IsKeyPressed(Window::EKey::KEY_F))
				{
					MoveToTarget(EDITOR_EXEC(GetSelectedActor()));
				}

				auto focusObjectFromAngle = [this, &targetPos, &dist](const glm::vec3& pOffset)
					{
						auto camPos = targetPos + pOffset * dist;
						auto direction = glm::normalize(targetPos - camPos);
						mCameraRotation = glm::lookAt(direction, glm::zero<glm::vec3>(), abs(direction.y) == 1.0f ? glm::right<glm::vec3>() : glm::up<glm::vec3>());
						mCameraDestinations.push({ camPos, mCameraRotation });
					};

				if (mInputManager.IsKeyPressed(Window::EKey::KEY_UP))			focusObjectFromAngle(glm::up<glm::vec3>());
				if (mInputManager.IsKeyPressed(Window::EKey::KEY_DOWN))			focusObjectFromAngle(-glm::up<glm::vec3>());
				if (mInputManager.IsKeyPressed(Window::EKey::KEY_RIGHT))		focusObjectFromAngle(glm::right<glm::vec3>());
				if (mInputManager.IsKeyPressed(Window::EKey::KEY_LEFT))			focusObjectFromAngle(-glm::right<glm::vec3>());
				if (mInputManager.IsKeyPressed(Window::EKey::KEY_PAGE_UP))		focusObjectFromAngle(glm::forward<glm::vec3>());
				if (mInputManager.IsKeyPressed(Window::EKey::KEY_PAGE_DOWN))	focusObjectFromAngle(-glm::forward<glm::vec3>());
			}
		}
	}

	if (!mCameraDestinations.empty())
	{
		mCurrentMovementSpeed = glm::zero<glm::vec3>();

		while (mCameraDestinations.size() != 1)
		{
			mCameraDestinations.pop();
		}

		auto& [destPos, destRotation] = mCameraDestinations.front();

		float t = mFocusLerpCoefficient * pDeltaTime;

		if (glm::distance(mCameraPosition, destPos) <= 0.03f)
		{
			mCameraPosition = destPos;
			mCameraRotation = destRotation;
			mCameraDestinations.pop();
		}
		else
		{
			mCameraPosition = glm::lerp(mCameraPosition, destPos, t);
			mCameraRotation = glm::lerp(mCameraRotation, destRotation, t);
		}
	}
	else
	{
		if (mRightMousePressed || mMiddleMousePressed || mLeftMousePressed)
		{
			auto [xPos, yPos] = mInputManager.GetMousePosition();

			bool wasFirstMouse = mFirstMouse;

			if (mFirstMouse)
			{
				mLastMousePosX = xPos;
				mLastMousePosY = yPos;
				mFirstMouse = false;
			}

			glm::vec2 mouseOffset
			{
				static_cast<float>(xPos - mLastMousePosX),
				static_cast<float>(mLastMousePosY - yPos)
			};

			mLastMousePosX = xPos;
			mLastMousePosY = yPos;

			if (mRightMousePressed)
			{
				HandleCameraFPSMouse(mouseOffset, wasFirstMouse);
			}
			else
			{
				if (mMiddleMousePressed)
				{
					if (mInputManager.GetKeyState(Window::EKey::KEY_LEFT_ALT) == Window::EKeyState::KEY_DOWN)
					{
						if (EDITOR_EXEC(IsAnyActorSelected()))
						{
							HandleCameraOrbit(mouseOffset, wasFirstMouse);
						}
					}
					else
					{
						HandleCameraPanning(mouseOffset, wasFirstMouse);
					}
				}
			}
		}

		if (mView.IsHovered())
		{
			HandleCameraZoom();
		}

		HandleCameraFPSKeyboard(pDeltaTime);
	}
}

void Editor::Editor_CameraController::MoveToTarget(Core::Core_Actor& pTarget)
{
	mCameraDestinations.push({ pTarget.transform.GetWorldPosition() - mCameraRotation * glm::vec3(0.f, 0.f, 1.f)* GetActorFocusDist(pTarget), mCameraRotation });
}

void Editor::Editor_CameraController::SetSpeed(float pSpeed)
{
	mCameraMoveSpeed = pSpeed;
}

float Editor::Editor_CameraController::GetSpeed() const
{
	return mCameraMoveSpeed;
}

void Editor::Editor_CameraController::SetPosition(const glm::vec3& pPosition)
{
	mCameraPosition = pPosition;
}

void Editor::Editor_CameraController::SetRotation(const glm::quat& pRotation)
{
	mCameraRotation = pRotation;
}

const glm::vec3& Editor::Editor_CameraController::GetPosition() const
{
	return mCameraPosition;
}

const glm::quat& Editor::Editor_CameraController::GetRotation() const
{
	return mCameraRotation;
}

bool Editor::Editor_CameraController::IsRightMousePressed() const
{
	return mRightMousePressed;
}

void Editor::Editor_CameraController::HandleCameraPanning(const glm::vec2& pMouseOffset, bool pFirstMouse)
{
	mWindow.SetCursorShape(Window::ECursorShape::HAND);

	auto mouseOffset = pMouseOffset * mCameraDragSpeed;

	mCameraPosition += mCameraRotation * glm::right<glm::vec3>() * mouseOffset.x;
	mCameraPosition -= mCameraRotation * glm::up<glm::vec3>() * mouseOffset.y;
}

glm::vec3 RemoveRoll(const glm::vec3& p_ypr)
{
	glm::vec3 result = p_ypr;

	if (result.z >= 179.0f || result.z <= -179.0f)
	{
		result.x += result.z;
		result.y = 180.0f - result.y;
		result.z = 0.0f;
	}

	if (result.x > 180.0f) result.x -= 360.0f;
	if (result.x < -180.0f) result.x += 360.0f;

	return result;
}

void Editor::Editor_CameraController::HandleCameraOrbit(const glm::vec2& pMouseOffset, bool pFirstMouse)
{
	auto mouseOffset = pMouseOffset * mCameraOrbitSpeed;

	if (pFirstMouse)
	{
		mYPR = glm::eulerAngles(mCameraRotation);
		mYPR = RemoveRoll(mYPR);
		mOrbitTarget = &EDITOR_EXEC(GetSelectedActor()).transform.GetFTransform();
		mOrbitStartOffset = -glm::up<glm::vec3>() * glm::distance(mOrbitTarget->GetWorldPosition(), mCameraPosition);
	}

	mYPR.y += -mouseOffset.x;
	mYPR.x += -mouseOffset.y;
	mYPR.x = std::max(std::min(mYPR.x, 90.0f), -90.0f);

	auto& target = EDITOR_EXEC(GetSelectedActor()).transform.GetFTransform();
	Maths::Maths_FTransform pivotTransform(target.GetWorldPosition());
	Maths::Maths_FTransform cameraTransform(mOrbitStartOffset);
	cameraTransform.SetParent(pivotTransform);
	pivotTransform.RotateLocal(glm::quat(mYPR));
	mCameraPosition = cameraTransform.GetWorldPosition();
	mCameraRotation = cameraTransform.GetWorldRotation();
}

void Editor::Editor_CameraController::HandleCameraFPSMouse(const glm::vec2& pMouseOffset, bool pFirstMouse)
{
	auto mouseOffset = pMouseOffset * mMouseSensitivity;

	if (pFirstMouse)
	{
		mYPR = glm::eulerAngles(mCameraRotation);
		mYPR = RemoveRoll(mYPR);
	}

	mYPR.y -= mouseOffset.x;
	mYPR.x += -mouseOffset.y;
	mYPR.x = std::max(std::min(mYPR.x, 90.0f), -90.0f);

	mCameraRotation = glm::quat(mYPR);
}

void Editor::Editor_CameraController::HandleCameraZoom()
{
	mCameraPosition += mCameraRotation * glm::up<glm::vec3>() * ImGui::GetIO().MouseWheel;
}

void Editor::Editor_CameraController::HandleCameraFPSKeyboard(float pDeltaTime)
{
	mTargetSpeed = glm::zero<glm::vec3>();

	if (mRightMousePressed)
	{
		bool run = mInputManager.GetKeyState(Window::EKey::KEY_LEFT_SHIFT) == Window::EKeyState::KEY_DOWN;
		float velocity = mCameraMoveSpeed * pDeltaTime * (run ? 2.0f : 1.0f);

		if (mInputManager.GetKeyState(Window::EKey::KEY_W) == Window::EKeyState::KEY_DOWN)
		{
			mTargetSpeed += mCameraRotation * glm::forward<glm::vec3>() * velocity;
		}
		if (mInputManager.GetKeyState(Window::EKey::KEY_S) == Window::EKeyState::KEY_DOWN)
		{
			mTargetSpeed += mCameraRotation * glm::forward<glm::vec3>() * -velocity;
		}
		if (mInputManager.GetKeyState(Window::EKey::KEY_A) == Window::EKeyState::KEY_DOWN)
		{
			mTargetSpeed += mCameraRotation * glm::right<glm::vec3>() * velocity;
		}
		if (mInputManager.GetKeyState(Window::EKey::KEY_D) == Window::EKeyState::KEY_DOWN)
		{
			mTargetSpeed += mCameraRotation * glm::right<glm::vec3>() * -velocity;
		}
		if (mInputManager.GetKeyState(Window::EKey::KEY_E) == Window::EKeyState::KEY_DOWN)
		{
			mTargetSpeed += glm::vec3{0.0f, velocity, 0.0f};
		}
		if (mInputManager.GetKeyState(Window::EKey::KEY_Q) == Window::EKeyState::KEY_DOWN)
		{
			mTargetSpeed += glm::vec3{0.0f, -velocity, 0.0f};
		}
	}

	mCurrentMovementSpeed = glm::lerp(mCurrentMovementSpeed, mTargetSpeed, 10.0f * pDeltaTime);
	mCameraPosition += mCurrentMovementSpeed;
}

void Editor::Editor_CameraController::UpdateMouseState()
{
	if (mInputManager.IsMouseButtonPressed(Window::EMouseButton::MOUSE_BUTTON_LEFT))
	{
		mLeftMousePressed = true;
	}

	if (mInputManager.IsMouseButtonReleased(Window::EMouseButton::MOUSE_BUTTON_LEFT))
	{
		mLeftMousePressed = false;
		mFirstMouse = true;
	}

	if (mInputManager.IsMouseButtonPressed(Window::EMouseButton::MOUSE_BUTTON_MIDDLE))
	{
		mMiddleMousePressed = true;
	}

	if (mInputManager.IsMouseButtonReleased(Window::EMouseButton::MOUSE_BUTTON_MIDDLE))
	{
		mMiddleMousePressed = false;
		mFirstMouse = true;
	}

	if (mInputManager.IsMouseButtonPressed(Window::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		mRightMousePressed = true;
		mWindow.SetCursorMode(Window::ECursorMode::DISABLED);
	}

	if (mInputManager.IsMouseButtonReleased(Window::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		mRightMousePressed = false;
		mFirstMouse = true;
		mWindow.SetCursorMode(Window::ECursorMode::NORMAL);
	}
}
