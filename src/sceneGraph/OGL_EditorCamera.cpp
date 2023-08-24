#include "OGL_EditorCamera.h"

using namespace OGL;

OGL_EditorCamera::OGL_EditorCamera() 
{
	UpdateCameraVectors();
}

void OGL_EditorCamera::ProcessMouseMovement(double xoffset, double yoffset, bool constrainPitch)
{
	xoffset *= mMouseSenesitivity;
	yoffset *= mMouseSenesitivity;

	mYaw += xoffset;
	mPitch -= yoffset;

	if (constrainPitch)
	{
		if (mPitch > 89.0f)
		{
			mPitch = 89.0f;
		}
		if (mPitch < -89.0f)
		{
			mPitch = -89.0f;
		}
	}

	UpdateCameraVectors();
}

void OGL_EditorCamera::ProcessMouseScroll(double yoffset)
{
	mZoom -= yoffset * mZoomSpeed;
	if (mZoom < 1.0f)
	{
		mZoom = 1.0f;
	}
	if (mZoom > 60.0f)
	{
		mZoom = 60.0f;
	}
}

void OGL_EditorCamera::ProcessKeyboard(CameraMovement dir, double dt)
{
	float velocity = mMovementSpeed * dt;
	switch (dir)
	{
	case OGL::CameraMovement::Forward:
		mPosition += mFront * velocity;
		break;
	case OGL::CameraMovement::Backward:
		mPosition -= mFront * velocity;
		break;
	case OGL::CameraMovement::Left:
		mPosition -= mRight * velocity;
		break;
	case OGL::CameraMovement::Right:
		mPosition += mRight * velocity;
		break;
	default:
		break;
	}
}

void OGL_EditorCamera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));
	front.y = glm::sin(glm::radians(mPitch));
	front.z = glm::sin(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}