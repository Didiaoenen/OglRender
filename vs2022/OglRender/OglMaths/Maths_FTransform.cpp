#include "Maths_FTransform.h"

Maths::Maths_FTransform::Maths_FTransform(glm::vec3 p_localPosition, glm::quat p_localRotation, glm::vec3 p_localScale)
{
}

Maths::Maths_FTransform::~Maths_FTransform()
{
}

void Maths::Maths_FTransform::NotificationHandler(Maths_TransformNotifier::ENotification p_notification)
{
}

void Maths::Maths_FTransform::SetParent(Maths_FTransform& p_parent)
{
}

bool Maths::Maths_FTransform::RemoveParent()
{
	return false;
}

bool Maths::Maths_FTransform::HasParent() const
{
	return false;
}

void Maths::Maths_FTransform::GenerateMatricesWorld(glm::vec3 p_position, glm::quat p_rotation, glm::vec3 p_scale)
{
}

void Maths::Maths_FTransform::GenerateMatricesLocal(glm::vec3 p_position, glm::quat p_rotation, glm::vec3 p_scale)
{
}

void Maths::Maths_FTransform::UpdateWorldMatrix()
{
}

void Maths::Maths_FTransform::UpdateLocalMatrix()
{
}

void Maths::Maths_FTransform::SetLocalPosition(glm::vec3 p_newPosition)
{
}

void Maths::Maths_FTransform::SetLocalRotation(glm::quat p_newRotation)
{
}

void Maths::Maths_FTransform::SetLocalScale(glm::vec3 p_newScale)
{
}

void Maths::Maths_FTransform::SetWorldPosition(glm::vec3 p_newPosition)
{
}

void Maths::Maths_FTransform::SetWorldRotation(glm::quat p_newRotation)
{
}

void Maths::Maths_FTransform::SetWorldScale(glm::vec3 p_newScale)
{
}

void Maths::Maths_FTransform::TranslateLocal(const glm::vec3& p_translation)
{
}

void Maths::Maths_FTransform::RotateLocal(const glm::quat& p_rotation)
{
}

void Maths::Maths_FTransform::ScaleLocal(const glm::vec3& p_scale)
{
}

const glm::vec3& Maths::Maths_FTransform::GetLocalPosition() const
{
	return glm::vec3();
}

const glm::quat& Maths::Maths_FTransform::GetLocalRotation() const
{
	return glm::quat();
}

const glm::vec3& Maths::Maths_FTransform::GetLocalScale() const
{
	return glm::vec3();
}

const glm::vec3& Maths::Maths_FTransform::GetWorldPosition() const
{
	return glm::vec3();
}

const glm::quat& Maths::Maths_FTransform::GetWorldRotation() const
{
	return glm::quat();
}

const glm::vec3& Maths::Maths_FTransform::GetWorldScale() const
{
	return glm::vec3();
}

const glm::mat4& Maths::Maths_FTransform::GetLocalMatrix() const
{
	return glm::mat4();
}

const glm::mat4& Maths::Maths_FTransform::GetWorldMatrix() const
{
	return glm::mat4();
}

glm::vec3 Maths::Maths_FTransform::GetWorldForward() const
{
	return glm::vec3();
}

glm::vec3 Maths::Maths_FTransform::GetWorldUp() const
{
	return glm::vec3();
}

glm::vec3 Maths::Maths_FTransform::GetWorldRight() const
{
	return glm::vec3();
}

glm::vec3 Maths::Maths_FTransform::GetLocalForward() const
{
	return glm::vec3();
}

glm::vec3 Maths::Maths_FTransform::GetLocalUp() const
{
	return glm::vec3();
}

glm::vec3 Maths::Maths_FTransform::GetLocalRight() const
{
	return glm::vec3();
}

void Maths::Maths_FTransform::PreDecomposeWorldMatrix()
{
}

void Maths::Maths_FTransform::PreDecomposeLocalMatrix()
{
}
