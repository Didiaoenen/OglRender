#include "Core_CTransform.h"

Core::Core_CTransform::Core_CTransform(Core_Actor& pOwner, glm::vec3 pLocalPosition, glm::quat pLocalRotation, glm::vec3 pLocalScale) :
	Core_AComponent(pOwner)
{
	mTransform.GenerateMatricesLocal(pLocalPosition, pLocalRotation, pLocalScale);
}

std::string Core::Core_CTransform::GetName()
{
	return "Transform";
}

void Core::Core_CTransform::SetParent(Core_CTransform& pParent)
{
	mTransform.SetParent(pParent.GetFTransform());
}

bool Core::Core_CTransform::RemoveParent()
{
	return mTransform.RemoveParent();
}

bool Core::Core_CTransform::HasParent() const
{
	return mTransform.HasParent();
}

void Core::Core_CTransform::SetLocalPosition(glm::vec3 pNewPosition)
{
	mTransform.SetLocalPosition(pNewPosition);
}

void Core::Core_CTransform::SetLocalRotation(glm::quat pNewRotation)
{
	mTransform.SetLocalRotation(pNewRotation);
}

void Core::Core_CTransform::SetLocalScale(glm::vec3 pNewScale)
{
	mTransform.SetLocalScale(pNewScale);
}

void Core::Core_CTransform::SetWorldPosition(glm::vec3 pNewPosition)
{
	mTransform.SetWorldPosition(pNewPosition);
}

void Core::Core_CTransform::SetWorldRotation(glm::quat pNewRotation)
{
	mTransform.SetWorldRotation(pNewRotation);
}

void Core::Core_CTransform::SetWorldScale(glm::vec3 pNewScale)
{
	mTransform.SetWorldScale(pNewScale);
}

void Core::Core_CTransform::TranslateLocal(const glm::vec3& pTranslation)
{
	mTransform.TranslateLocal(pTranslation);
}

void Core::Core_CTransform::RotateLocal(const glm::quat& pRotation)
{
	mTransform.RotateLocal(pRotation);
}

void Core::Core_CTransform::ScaleLocal(const glm::vec3& pScale)
{
	mTransform.ScaleLocal(pScale);
}

const glm::vec3& Core::Core_CTransform::GetLocalPosition() const
{
	return mTransform.GetLocalPosition();
}

const glm::quat& Core::Core_CTransform::GetLocalRotation() const
{
	return mTransform.GetLocalRotation();
}

const glm::vec3& Core::Core_CTransform::GetLocalScale() const
{
	return mTransform.GetLocalScale();
}

const glm::vec3& Core::Core_CTransform::GetWorldPosition() const
{
	return mTransform.GetWorldPosition();
}

const glm::quat& Core::Core_CTransform::GetWorldRotation() const
{
	return mTransform.GetWorldRotation();
}

const glm::vec3& Core::Core_CTransform::GetWorldScale() const
{
	return mTransform.GetWorldScale();
}

const glm::mat4& Core::Core_CTransform::GetLocalMatrix() const
{
	return mTransform.GetLocalMatrix();
}

const glm::mat4& Core::Core_CTransform::GetWorldMatrix() const
{
	return mTransform.GetWorldMatrix();
}

Maths::Maths_FTransform& Core::Core_CTransform::GetFTransform()
{
	return mTransform;
}

glm::vec3 Core::Core_CTransform::GetWorldForward() const
{
	return mTransform.GetWorldForward();
}

glm::vec3 Core::Core_CTransform::GetWorldUp() const
{
	return mTransform.GetWorldUp();
}

glm::vec3 Core::Core_CTransform::GetWorldRight() const
{
	return mTransform.GetWorldRight();
}

glm::vec3 Core::Core_CTransform::GetLocalForward() const
{
	return mTransform.GetLocalForward();
}

glm::vec3 Core::Core_CTransform::GetLocalUp() const
{
	return mTransform.GetLocalUp();
}

glm::vec3 Core::Core_CTransform::GetLocalRight() const
{
	return mTransform.GetLocalRight();
}

void Core::Core_CTransform::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	Core_Serializer::SerializeVec3(pDoc, pNode, "position", GetLocalPosition());
	Core_Serializer::SerializeQuat(pDoc, pNode, "rotation", GetLocalRotation());
	Core_Serializer::SerializeVec3(pDoc, pNode, "scale", GetLocalScale());
}

void Core::Core_CTransform::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
	mTransform.GenerateMatricesLocal
	(
		Core_Serializer::DeserializeVec3(pDoc, pNode, "position"),
		Core_Serializer::DeserializeQuat(pDoc, pNode, "rotation"),
		Core_Serializer::DeserializeVec3(pDoc, pNode, "scale")
	);
}

void Core::Core_CTransform::OnInspector(UI::UI_WidgetContainer& pRoot)
{
	auto getRotation = [this]
		{
			return glm::eulerAngles(GetLocalRotation());
		};

	auto setRotation = [this](glm::vec3 result)
		{
			SetLocalRotation(glm::quat(result));
		};

	Core_GUIDrawer::DrawVec3(pRoot, "Position", std::bind(&Core_CTransform::GetLocalPosition, this), std::bind(&Core_CTransform::SetLocalPosition, this, std::placeholders::_1), 0.05f);
	Core_GUIDrawer::DrawVec3(pRoot, "Rotation", getRotation, setRotation, 0.05f);
	Core_GUIDrawer::DrawVec3(pRoot, "Scale", std::bind(&Core_CTransform::GetLocalScale, this), std::bind(&Core_CTransform::SetLocalScale, this, std::placeholders::_1), 0.05f, 0.0001f);
}
