#include "Maths_FTransform.h"

Maths::Maths_FTransform::Maths_FTransform(glm::vec3 pLocalPosition, glm::quat pLocalRotation, glm::vec3 pLocalScale) :
	mNotificationHandlerID(-1),
	mParent(nullptr)
{
	GenerateMatricesLocal(pLocalPosition, pLocalRotation, pLocalScale);
}

Maths::Maths_FTransform::~Maths_FTransform()
{
	mNotifier.NotifyChildren(Maths_TransformNotifier::ENotification::TRANSFORM_DESTROYED);
}

void Maths::Maths_FTransform::NotificationHandler(Maths_TransformNotifier::ENotification pNotification)
{
	switch (pNotification)
	{
	case Maths_TransformNotifier::ENotification::TRANSFORM_CHANGED:
		UpdateWorldMatrix();
		break;

	case Maths_TransformNotifier::ENotification::TRANSFORM_DESTROYED:
		GenerateMatricesLocal(mWorldPosition, mWorldRotation, mWorldScale);
		mParent = nullptr;
		UpdateWorldMatrix();
		break;
	}
}

void Maths::Maths_FTransform::SetParent(Maths_FTransform& pParent)
{
	mParent = &pParent;

	mNotificationHandlerID = mParent->mNotifier.AddNotificationHandler(std::bind(&Maths_FTransform::NotificationHandler, this, std::placeholders::_1));

	UpdateWorldMatrix();
}

bool Maths::Maths_FTransform::RemoveParent()
{
	if (mParent != nullptr)
	{
		mParent->mNotifier.RemoveNotificationHandler(mNotificationHandlerID);
		mParent = nullptr;
		UpdateWorldMatrix();

		return true;
	}

	return false;
}

bool Maths::Maths_FTransform::HasParent() const
{
	return mParent != nullptr;
}

void Maths::Maths_FTransform::GenerateMatricesWorld(glm::vec3 pPosition, glm::quat pRotation, glm::vec3 pScale)
{
	mLocalMatrix = glm::translate(glm::identity<glm::mat4>(), pPosition) * glm::mat4(glm::normalize(pRotation)) * glm::scale(glm::identity<glm::mat4>(), pScale);
	mLocalPosition = pPosition;
	mLocalRotation = pRotation;
	mLocalScale = pScale;

	UpdateWorldMatrix();
}

void Maths::Maths_FTransform::GenerateMatricesLocal(glm::vec3 pPosition, glm::quat pRotation, glm::vec3 pScale)
{
	mWorldMatrix = glm::translate(glm::identity<glm::mat4>(), pPosition) * glm::mat4(glm::normalize(pRotation)) * glm::scale(glm::identity<glm::mat4>(), pScale);
	mWorldPosition = pPosition;
	mWorldRotation = pRotation;
	mWorldScale = pScale;

	UpdateLocalMatrix();
}

void Maths::Maths_FTransform::UpdateWorldMatrix()
{
	mWorldMatrix = HasParent() ? mParent->mWorldMatrix * mLocalMatrix : mLocalMatrix;
	PreDecomposeWorldMatrix();

	mNotifier.NotifyChildren(Maths_TransformNotifier::ENotification::TRANSFORM_CHANGED);
}

void Maths::Maths_FTransform::UpdateLocalMatrix()
{
	mLocalMatrix = HasParent() ? glm::inverse(mParent->mWorldMatrix) * mWorldMatrix : mWorldMatrix;
	PreDecomposeLocalMatrix();

	mNotifier.NotifyChildren(Maths_TransformNotifier::ENotification::TRANSFORM_CHANGED);
}

void Maths::Maths_FTransform::SetLocalPosition(glm::vec3 pNewPosition)
{
	GenerateMatricesLocal(pNewPosition, mLocalRotation, mLocalScale);
}

void Maths::Maths_FTransform::SetLocalRotation(glm::quat pNewRotation)
{
	GenerateMatricesLocal(mLocalPosition, pNewRotation, mLocalScale);
}

void Maths::Maths_FTransform::SetLocalScale(glm::vec3 pNewScale)
{
	GenerateMatricesLocal(mLocalPosition, mLocalRotation, pNewScale);
}

void Maths::Maths_FTransform::SetWorldPosition(glm::vec3 pNewPosition)
{
	GenerateMatricesWorld(pNewPosition, mWorldRotation, mWorldScale);
}

void Maths::Maths_FTransform::SetWorldRotation(glm::quat pNewRotation)
{
	GenerateMatricesWorld(mWorldPosition, pNewRotation, mWorldScale);
}

void Maths::Maths_FTransform::SetWorldScale(glm::vec3 pNewScale)
{
	GenerateMatricesWorld(mWorldPosition, mWorldRotation, pNewScale);
}

void Maths::Maths_FTransform::TranslateLocal(const glm::vec3& pTranslation)
{
	SetLocalPosition(mLocalPosition + pTranslation);
}

void Maths::Maths_FTransform::RotateLocal(const glm::quat& pRotation)
{
	SetLocalRotation(mLocalRotation * pRotation);
}

void Maths::Maths_FTransform::ScaleLocal(const glm::vec3& pScale)
{
	SetLocalScale(glm::vec3
	(
		mLocalScale.x * pScale.x,
		mLocalScale.y * pScale.y,
		mLocalScale.z * pScale.z
	));
}

const glm::vec3& Maths::Maths_FTransform::GetLocalPosition() const
{
	return mLocalPosition;
}

const glm::quat& Maths::Maths_FTransform::GetLocalRotation() const
{
	return mLocalRotation;
}

const glm::vec3& Maths::Maths_FTransform::GetLocalScale() const
{
	return mLocalScale;
}

const glm::vec3& Maths::Maths_FTransform::GetWorldPosition() const
{
	return mWorldPosition;
}

const glm::quat& Maths::Maths_FTransform::GetWorldRotation() const
{
	return mWorldRotation;
}

const glm::vec3& Maths::Maths_FTransform::GetWorldScale() const
{
	return mWorldScale;
}

const glm::mat4& Maths::Maths_FTransform::GetLocalMatrix() const
{
	return mLocalMatrix;
}

const glm::mat4& Maths::Maths_FTransform::GetWorldMatrix() const
{
	return mWorldMatrix;
}

glm::vec3 Maths::Maths_FTransform::GetWorldForward() const
{
	return mWorldRotation * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Maths::Maths_FTransform::GetWorldUp() const
{
	return mWorldRotation * glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Maths::Maths_FTransform::GetWorldRight() const
{
	return mWorldRotation * glm::vec3(1.f, 0.f, 0.f);
}

glm::vec3 Maths::Maths_FTransform::GetLocalForward() const
{
	return mLocalRotation * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Maths::Maths_FTransform::GetLocalUp() const
{
	return mLocalRotation * glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Maths::Maths_FTransform::GetLocalRight() const
{
	return mLocalRotation * glm::vec3(1.f, 0.f, 0.f);
}

void Maths::Maths_FTransform::PreDecomposeWorldMatrix()
{
	mWorldPosition.x = mWorldMatrix[0][3];
	mWorldPosition.y = mWorldMatrix[1][3];
	mWorldPosition.z = mWorldMatrix[2][3];

	glm::vec3 columns[3] =
	{
		{ mWorldMatrix[0][0], mWorldMatrix[1][0], mWorldMatrix[2][0]},
		{ mWorldMatrix[0][1], mWorldMatrix[1][1], mWorldMatrix[2][1]},
		{ mWorldMatrix[0][2], mWorldMatrix[1][2], mWorldMatrix[2][2]},
	};

	mWorldScale.x = glm::length(columns[0]);
	mWorldScale.y = glm::length(columns[1]);
	mWorldScale.z = glm::length(columns[2]);

	if (mWorldScale.x)
	{
		columns[0] /= mWorldScale.x;
	}
	if (mWorldScale.y)
	{
		columns[1] /= mWorldScale.y;
	}
	if (mWorldScale.z)
	{
		columns[2] /= mWorldScale.z;
	}

	glm::mat3 rotationMatrix
	(
		columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z
	);

	mWorldRotation = glm::quat(rotationMatrix);
}

void Maths::Maths_FTransform::PreDecomposeLocalMatrix()
{
	mLocalPosition.x = mLocalMatrix[0][3];
	mLocalPosition.y = mLocalMatrix[1][3];
	mLocalPosition.z = mLocalMatrix[2][3];

	glm::vec3 columns[3] =
	{
		{ mLocalMatrix[0][0], mLocalMatrix[1][0], mLocalMatrix[2][0]},
		{ mLocalMatrix[0][1], mLocalMatrix[1][1], mLocalMatrix[2][1]},
		{ mLocalMatrix[0][2], mLocalMatrix[1][2], mLocalMatrix[2][2]},
	};

	mLocalScale.x = glm::length(columns[0]);
	mLocalScale.y = glm::length(columns[1]);
	mLocalScale.z = glm::length(columns[2]);

	if (mLocalScale.x)
	{
		columns[0] /= mLocalScale.x;
	}
	if (mLocalScale.y)
	{
		columns[1] /= mLocalScale.y;
	}
	if (mLocalScale.z)
	{
		columns[2] /= mLocalScale.z;
	}

	glm::mat3 rotationMatrix
	(
		columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z
	);

	mLocalRotation = glm::quat(rotationMatrix);
}
