#include "Core_CTransform.h"

Core::Core_CTransform::Core_CTransform(Core_Actor& p_owner, glm::vec3 p_localPosition, glm::quat p_localRotation, glm::vec3 p_localScale) :
	Core_AComponent(p_owner)
{
	m_transform.GenerateMatricesLocal(p_localPosition, p_localRotation, p_localScale);
}

std::string Core::Core_CTransform::GetName()
{
	return "Transform";
}

void Core::Core_CTransform::SetParent(Core_CTransform& p_parent)
{
	m_transform.SetParent(p_parent.GetFTransform());
}

bool Core::Core_CTransform::RemoveParent()
{
	return m_transform.RemoveParent();
}

bool Core::Core_CTransform::HasParent() const
{
	return m_transform.HasParent();
}

void Core::Core_CTransform::SetLocalPosition(glm::vec3 p_newPosition)
{
	m_transform.SetLocalPosition(p_newPosition);
}

void Core::Core_CTransform::SetLocalRotation(glm::quat p_newRotation)
{
	m_transform.SetLocalRotation(p_newRotation);
}

void Core::Core_CTransform::SetLocalScale(glm::vec3 p_newScale)
{
	m_transform.SetLocalScale(p_newScale);
}

void Core::Core_CTransform::SetWorldPosition(glm::vec3 p_newPosition)
{
	m_transform.SetWorldPosition(p_newPosition);
}

void Core::Core_CTransform::SetWorldRotation(glm::quat p_newRotation)
{
	m_transform.SetWorldRotation(p_newRotation);
}

void Core::Core_CTransform::SetWorldScale(glm::vec3 p_newScale)
{
	m_transform.SetWorldScale(p_newScale);
}

void Core::Core_CTransform::TranslateLocal(const glm::vec3& p_translation)
{
	m_transform.TranslateLocal(p_translation);
}

void Core::Core_CTransform::RotateLocal(const glm::quat& p_rotation)
{
	m_transform.RotateLocal(p_rotation);
}

void Core::Core_CTransform::ScaleLocal(const glm::vec3& p_scale)
{
	m_transform.ScaleLocal(p_scale);
}

const glm::vec3& Core::Core_CTransform::GetLocalPosition() const
{
	return m_transform.GetLocalPosition();
}

const glm::quat& Core::Core_CTransform::GetLocalRotation() const
{
	return m_transform.GetLocalRotation();
}

const glm::vec3& Core::Core_CTransform::GetLocalScale() const
{
	return m_transform.GetLocalScale();
}

const glm::vec3& Core::Core_CTransform::GetWorldPosition() const
{
	return m_transform.GetWorldPosition();
}

const glm::quat& Core::Core_CTransform::GetWorldRotation() const
{
	return m_transform.GetWorldRotation();
}

const glm::vec3& Core::Core_CTransform::GetWorldScale() const
{
	return m_transform.GetWorldScale();
}

const glm::mat4& Core::Core_CTransform::GetLocalMatrix() const
{
	return m_transform.GetLocalMatrix();
}

const glm::mat4& Core::Core_CTransform::GetWorldMatrix() const
{
	return m_transform.GetWorldMatrix();
}

Maths::Maths_FTransform& Core::Core_CTransform::GetFTransform()
{
	return m_transform;
}

glm::vec3 Core::Core_CTransform::GetWorldForward() const
{
	return m_transform.GetWorldForward();
}

glm::vec3 Core::Core_CTransform::GetWorldUp() const
{
	return m_transform.GetWorldUp();
}

glm::vec3 Core::Core_CTransform::GetWorldRight() const
{
	return m_transform.GetWorldRight();
}

glm::vec3 Core::Core_CTransform::GetLocalForward() const
{
	return m_transform.GetLocalForward();
}

glm::vec3 Core::Core_CTransform::GetLocalUp() const
{
	return m_transform.GetLocalUp();
}

glm::vec3 Core::Core_CTransform::GetLocalRight() const
{
	return m_transform.GetLocalRight();
}

void Core::Core_CTransform::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Core_Serializer::SerializeVec3(p_doc, p_node, "position", GetLocalPosition());
	Core_Serializer::SerializeQuat(p_doc, p_node, "rotation", GetLocalRotation());
	Core_Serializer::SerializeVec3(p_doc, p_node, "scale", GetLocalScale());
}

void Core::Core_CTransform::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	m_transform.GenerateMatricesLocal
	(
		Core_Serializer::DeserializeVec3(p_doc, p_node, "position"),
		Core_Serializer::DeserializeQuat(p_doc, p_node, "rotation"),
		Core_Serializer::DeserializeVec3(p_doc, p_node, "scale")
	);
}

void Core::Core_CTransform::OnInspector(UI::UI_WidgetContainer& p_root)
{
	auto getRotation = [this]
		{
			return glm::eulerAngles(GetLocalRotation());
		};

	auto setRotation = [this](glm::vec3 result)
		{
			SetLocalRotation(glm::quat(result));
		};

	Core_GUIDrawer::DrawVec3(p_root, "Position", std::bind(&Core_CTransform::GetLocalPosition, this), std::bind(&Core_CTransform::SetLocalPosition, this, std::placeholders::_1), 0.05f);
	Core_GUIDrawer::DrawVec3(p_root, "Rotation", getRotation, setRotation, 0.05f);
	Core_GUIDrawer::DrawVec3(p_root, "Scale", std::bind(&Core_CTransform::GetLocalScale, this), std::bind(&Core_CTransform::SetLocalScale, this, std::placeholders::_1), 0.05f, 0.0001f);
}
