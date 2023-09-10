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
		Core_CTransform(Core_Actor& p_owner, glm::vec3 p_localPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::quat p_localRotation = glm::identity<glm::quat>(), glm::vec3 p_localScale = glm::vec3(1.0f, 1.0f, 1.0f));

		std::string GetName() override;

		void SetParent(Core_CTransform& p_parent);

		bool RemoveParent();

		bool HasParent() const;

		void SetLocalPosition(glm::vec3 p_newPosition);

		void SetLocalRotation(glm::quat p_newRotation);

		void SetLocalScale(glm::vec3 p_newScale);

		void SetWorldPosition(glm::vec3 p_newPosition);

		void SetWorldRotation(glm::quat p_newRotation);

		void SetWorldScale(glm::vec3 p_newScale);

		void TranslateLocal(const glm::vec3& p_translation);

		void RotateLocal(const glm::quat& p_rotation);

		void ScaleLocal(const glm::vec3& p_scale);

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

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;

	private:
		Maths::Maths_FTransform m_transform;
	};
}

