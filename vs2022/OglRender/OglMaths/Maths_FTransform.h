#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Maths_TransformNotifier.h"

namespace Maths
{
	class Maths_FTransform
	{
	public:
		Maths_FTransform(glm::vec3 pLocalPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::quat pLocalRotation = glm::identity<glm::quat>(), glm::vec3 pLocalScale = glm::vec3(1.0f, 1.0f, 1.0f));

		~Maths_FTransform();

		void NotificationHandler(Maths_TransformNotifier::ENotification pNotification);

		void SetParent(Maths_FTransform& pParent);

		bool RemoveParent();

		bool HasParent() const;

		void GenerateMatricesWorld(glm::vec3 pPosition, glm::quat pRotation, glm::vec3 pScale);

		void GenerateMatricesLocal(glm::vec3 pPosition, glm::quat pRotation, glm::vec3 pScale);

		void UpdateWorldMatrix();

		void UpdateLocalMatrix();

		void SetLocalPosition(glm::vec3 pNewPosition);

		void SetLocalRotation(glm::quat pNewRotation);

		void SetLocalScale(glm::vec3 pNewScale);

		void SetWorldPosition(glm::vec3 pNewPosition);

		void SetWorldRotation(glm::quat pNewRotation);

		void SetWorldScale(glm::vec3 pNewScale);

		void TranslateLocal(const glm::vec3& pTranslation);

		void RotateLocal(const glm::quat& pRotation);

		void ScaleLocal(const glm::vec3& pScale);

		const glm::vec3& GetLocalPosition() const;

		const glm::quat& GetLocalRotation() const;

		const glm::vec3& GetLocalScale() const;

		const glm::vec3& GetWorldPosition() const;

		const glm::quat& GetWorldRotation() const;

		const glm::vec3& GetWorldScale() const;

		const glm::mat4& GetLocalMatrix() const;

		const glm::mat4& GetWorldMatrix() const;

		glm::vec3 GetWorldForward() const;

		glm::vec3 GetWorldUp() const;

		glm::vec3 GetWorldRight() const;

		glm::vec3 GetLocalForward() const;

		glm::vec3 GetLocalUp() const;

		glm::vec3 GetLocalRight() const;

	public:
		Maths_TransformNotifier Notifier;
		Maths_TransformNotifier::NotificationHandlerID m_notificationHandlerID;

	private:
		void PreDecomposeWorldMatrix();
		void PreDecomposeLocalMatrix();

		glm::vec3 m_localPosition;
		glm::quat m_localRotation;
		glm::vec3 m_localScale;
		glm::vec3 m_worldPosition;
		glm::quat m_worldRotation;
		glm::vec3 m_worldScale;

		glm::mat4 m_localMatrix;
		glm::mat4 m_worldMatrix;

		Maths_FTransform* m_parent;
	};
}

