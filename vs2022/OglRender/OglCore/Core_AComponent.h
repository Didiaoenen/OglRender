#pragma once

#include "Core_IInspectorItem.h"

namespace Core
{
	class Core_Actor;

	class Core_AComponent : public Core_IInspectorItem
	{
	public:
		Core_AComponent(Core_Actor& p_owner);

		virtual ~Core_AComponent();

		virtual void OnAwake() {}

		virtual void OnStart() {}

		virtual void OnEnable() {}

		virtual void OnDisable() {}

		virtual void OnDestroy() {}

		virtual void OnUpdate(float p_deltaTime) {}

		virtual void OnFixedUpdate(float p_deltaTime) {}

		virtual void OnLateUpdate(float p_deltaTime) {}

		//virtual void OnCollisionEnter(Core::CPhysicalObject& p_otherObject) {}

		//virtual void OnCollisionStay(Core::CPhysicalObject& p_otherObject) {}

		//virtual void OnCollisionExit(Core::CPhysicalObject& p_otherObject) {}

		//virtual void OnTriggerEnter(Core::CPhysicalObject& p_otherObject) {}

		//virtual void OnTriggerStay(Core::CPhysicalObject& p_otherObject) {}

		//virtual void OnTriggerExit(Core::CPhysicalObject& p_otherObject) {}

		virtual std::string GetName() = 0;

	public:
		Core_Actor& owner;
	};
}

