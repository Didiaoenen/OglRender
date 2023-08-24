#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "sceneGraph/OGL_Component.h"

namespace OGL
{
class OGL_Transform : public OGL_Component
{
public:
	OGL_Transform() = default;
	virtual ~OGL_Transform() = default;

	glm::mat4 GetTransform() const
	{
		return 
			glm::translate(mTransform, mTranslation) * glm::toMat4(glm::quat(mRotation)) * glm::scale(mTransform, mScale);
	}

public:
	glm::vec3 mTranslation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 mRotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 mScale{ 1.0f, 1.0f, 1.0f };
	glm::mat4 mTransform{ glm::identity<glm::mat4>() };
};
}