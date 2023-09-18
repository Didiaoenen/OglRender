#include "Maths_FTransform.h"

Maths::Maths_FTransform::Maths_FTransform(glm::vec3 pLocalPosition, glm::quat pLocalRotation, glm::vec3 pLocalScale) :
	m_notificationHandlerID(-1),
	m_parent(nullptr)
{
	GenerateMatricesLocal(pLocalPosition, pLocalRotation, pLocalScale);
}

Maths::Maths_FTransform::~Maths_FTransform()
{
	Notifier.NotifyChildren(Maths_TransformNotifier::ENotification::TRANSFORM_DESTROYED);
}

void Maths::Maths_FTransform::NotificationHandler(Maths_TransformNotifier::ENotification pNotification)
{
	switch (pNotification)
	{
	case Maths_TransformNotifier::ENotification::TRANSFORM_CHANGED:
		UpdateWorldMatrix();
		break;

	case Maths_TransformNotifier::ENotification::TRANSFORM_DESTROYED:
		GenerateMatricesLocal(m_worldPosition, m_worldRotation, m_worldScale);
		m_parent = nullptr;
		UpdateWorldMatrix();
		break;
	}
}

void Maths::Maths_FTransform::SetParent(Maths_FTransform& pParent)
{
	m_parent = &pParent;

	m_notificationHandlerID = m_parent->Notifier.AddNotificationHandler(std::bind(&Maths_FTransform::NotificationHandler, this, std::placeholders::_1));

	UpdateWorldMatrix();
}

bool Maths::Maths_FTransform::RemoveParent()
{
	if (m_parent != nullptr)
	{
		m_parent->Notifier.RemoveNotificationHandler(m_notificationHandlerID);
		m_parent = nullptr;
		UpdateWorldMatrix();

		return true;
	}

	return false;
}

bool Maths::Maths_FTransform::HasParent() const
{
	return m_parent != nullptr;
}

void Maths::Maths_FTransform::GenerateMatricesWorld(glm::vec3 pPosition, glm::quat pRotation, glm::vec3 pScale)
{
	m_localMatrix = glm::translate(glm::identity<glm::mat4>(), pPosition) * glm::mat4(glm::normalize(pRotation)) * glm::scale(glm::identity<glm::mat4>(), pScale);
	m_localPosition = pPosition;
	m_localRotation = pRotation;
	m_localScale = pScale;

	UpdateWorldMatrix();
}

void Maths::Maths_FTransform::GenerateMatricesLocal(glm::vec3 pPosition, glm::quat pRotation, glm::vec3 pScale)
{
	m_worldMatrix = glm::translate(glm::identity<glm::mat4>(), pPosition) * glm::mat4(glm::normalize(pRotation)) * glm::scale(glm::identity<glm::mat4>(), pScale);
	m_worldPosition = pPosition;
	m_worldRotation = pRotation;
	m_worldScale = pScale;

	UpdateLocalMatrix();
}

void Maths::Maths_FTransform::UpdateWorldMatrix()
{
	m_worldMatrix = HasParent() ? m_parent->m_worldMatrix * m_localMatrix : m_localMatrix;
	PreDecomposeWorldMatrix();

	Notifier.NotifyChildren(Maths_TransformNotifier::ENotification::TRANSFORM_CHANGED);
}

void Maths::Maths_FTransform::UpdateLocalMatrix()
{
	m_localMatrix = HasParent() ? glm::inverse(m_parent->m_worldMatrix) * m_worldMatrix : m_worldMatrix;
	PreDecomposeLocalMatrix();

	Notifier.NotifyChildren(Maths_TransformNotifier::ENotification::TRANSFORM_CHANGED);
}

void Maths::Maths_FTransform::SetLocalPosition(glm::vec3 pNewPosition)
{
	GenerateMatricesLocal(pNewPosition, m_localRotation, m_localScale);
}

void Maths::Maths_FTransform::SetLocalRotation(glm::quat pNewRotation)
{
	GenerateMatricesLocal(m_localPosition, pNewRotation, m_localScale);
}

void Maths::Maths_FTransform::SetLocalScale(glm::vec3 pNewScale)
{
	GenerateMatricesLocal(m_localPosition, m_localRotation, pNewScale);
}

void Maths::Maths_FTransform::SetWorldPosition(glm::vec3 pNewPosition)
{
	GenerateMatricesWorld(pNewPosition, m_worldRotation, m_worldScale);
}

void Maths::Maths_FTransform::SetWorldRotation(glm::quat pNewRotation)
{
	GenerateMatricesWorld(m_worldPosition, pNewRotation, m_worldScale);
}

void Maths::Maths_FTransform::SetWorldScale(glm::vec3 pNewScale)
{
	GenerateMatricesWorld(m_worldPosition, m_worldRotation, pNewScale);
}

void Maths::Maths_FTransform::TranslateLocal(const glm::vec3& pTranslation)
{
	SetLocalPosition(m_localPosition + pTranslation);
}

void Maths::Maths_FTransform::RotateLocal(const glm::quat& pRotation)
{
	SetLocalRotation(m_localRotation * pRotation);
}

void Maths::Maths_FTransform::ScaleLocal(const glm::vec3& pScale)
{
	SetLocalScale(glm::vec3
	(
		m_localScale.x * pScale.x,
		m_localScale.y * pScale.y,
		m_localScale.z * pScale.z
	));
}

const glm::vec3& Maths::Maths_FTransform::GetLocalPosition() const
{
	return m_localPosition;
}

const glm::quat& Maths::Maths_FTransform::GetLocalRotation() const
{
	return m_localRotation;
}

const glm::vec3& Maths::Maths_FTransform::GetLocalScale() const
{
	return m_localScale;
}

const glm::vec3& Maths::Maths_FTransform::GetWorldPosition() const
{
	return m_worldPosition;
}

const glm::quat& Maths::Maths_FTransform::GetWorldRotation() const
{
	return m_worldRotation;
}

const glm::vec3& Maths::Maths_FTransform::GetWorldScale() const
{
	return m_worldScale;
}

const glm::mat4& Maths::Maths_FTransform::GetLocalMatrix() const
{
	return m_localMatrix;
}

const glm::mat4& Maths::Maths_FTransform::GetWorldMatrix() const
{
	return m_worldMatrix;
}

glm::vec3 Maths::Maths_FTransform::GetWorldForward() const
{
	return m_worldRotation * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Maths::Maths_FTransform::GetWorldUp() const
{
	return m_worldRotation * glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Maths::Maths_FTransform::GetWorldRight() const
{
	return m_worldRotation * glm::vec3(1.f, 0.f, 0.f);
}

glm::vec3 Maths::Maths_FTransform::GetLocalForward() const
{
	return m_localRotation * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Maths::Maths_FTransform::GetLocalUp() const
{
	return m_localRotation * glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Maths::Maths_FTransform::GetLocalRight() const
{
	return m_localRotation * glm::vec3(1.f, 0.f, 0.f);
}

void Maths::Maths_FTransform::PreDecomposeWorldMatrix()
{
	m_worldPosition.x = m_worldMatrix[0][3];
	m_worldPosition.y = m_worldMatrix[1][3];
	m_worldPosition.z = m_worldMatrix[2][3];

	glm::vec3 columns[3] =
	{
		{ m_worldMatrix[0][0], m_worldMatrix[1][0], m_worldMatrix[2][0]},
		{ m_worldMatrix[0][1], m_worldMatrix[1][1], m_worldMatrix[2][1]},
		{ m_worldMatrix[0][2], m_worldMatrix[1][2], m_worldMatrix[2][2]},
	};

	m_worldScale.x = glm::length(columns[0]);
	m_worldScale.y = glm::length(columns[1]);
	m_worldScale.z = glm::length(columns[2]);

	if (m_worldScale.x)
	{
		columns[0] /= m_worldScale.x;
	}
	if (m_worldScale.y)
	{
		columns[1] /= m_worldScale.y;
	}
	if (m_worldScale.z)
	{
		columns[2] /= m_worldScale.z;
	}

	glm::mat3 rotationMatrix
	(
		columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z
	);

	m_worldRotation = glm::quat(rotationMatrix);
}

void Maths::Maths_FTransform::PreDecomposeLocalMatrix()
{
	m_localPosition.x = m_localMatrix[0][3];
	m_localPosition.y = m_localMatrix[1][3];
	m_localPosition.z = m_localMatrix[2][3];

	glm::vec3 columns[3] =
	{
		{ m_localMatrix[0][0], m_localMatrix[1][0], m_localMatrix[2][0]},
		{ m_localMatrix[0][1], m_localMatrix[1][1], m_localMatrix[2][1]},
		{ m_localMatrix[0][2], m_localMatrix[1][2], m_localMatrix[2][2]},
	};

	m_localScale.x = glm::length(columns[0]);
	m_localScale.y = glm::length(columns[1]);
	m_localScale.z = glm::length(columns[2]);

	if (m_localScale.x)
	{
		columns[0] /= m_localScale.x;
	}
	if (m_localScale.y)
	{
		columns[1] /= m_localScale.y;
	}
	if (m_localScale.z)
	{
		columns[2] /= m_localScale.z;
	}

	glm::mat3 rotationMatrix
	(
		columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z
	);

	m_localRotation = glm::quat(rotationMatrix);
}
