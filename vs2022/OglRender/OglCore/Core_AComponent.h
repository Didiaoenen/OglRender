#pragma once

#include "Core_IInspectorItem.h"

namespace Core
{
	class Core_Actor;

	class Core_AComponent : public Core_IInspectorItem
	{
	public:
		Core_AComponent(Core_Actor& pOwner);

		virtual ~Core_AComponent();

		virtual void OnAwake() {}

		virtual void OnStart() {}

		virtual void OnEnable() {}

		virtual void OnDisable() {}

		virtual void OnDestroy() {}

		virtual void OnUpdate(float pDeltaTime) {}

		virtual void OnFixedUpdate(float pDeltaTime) {}

		virtual void OnLateUpdate(float pDeltaTime) {}

		virtual std::string GetName() = 0;

	public:
		Core_Actor& mOwner;
	};
}

