#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>

#include <OglMaths/Maths_FTransform.h>

#include "Core_AComponent.h"

namespace Core
{
	class Core_Actor;

	class Core_CTransform : public Core_AComponent
	{
	public:
		Core_CTransform(Core_Actor& pOwner, glm::vec3 pLocalPosition = glm::zero<glm::vec3>(), glm::quat pLocalRotation = glm::identity<glm::quat>(), glm::vec3 pLocalScale = glm::one<glm::vec3>());

		std::string GetName() override;

		void SetParent(Core_CTransform& pParent);

		bool RemoveParent();

		bool HasParent() const;

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

		Maths::Maths_FTransform& GetFTransform();

		glm::vec3 GetWorldForward() const;

		glm::vec3 GetWorldUp() const;

		glm::vec3 GetWorldRight() const;

		glm::vec3 GetLocalForward() const;

		glm::vec3 GetLocalUp() const;

		glm::vec3 GetLocalRight() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;

	private:
		Maths::Maths_FTransform mTransform;
	};
}

