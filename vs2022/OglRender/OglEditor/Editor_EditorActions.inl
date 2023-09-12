#pragma once

#include "Editor_EditorActions.h"

namespace Editor
{
	template<typename T>
	inline Core::Core_Actor& Editor_EditorActions::CreateMonoComponentActor(bool p_focusOnCreation, Core::Core_Actor* pParent)
	{
		auto& instance = CreateEmptyActor(false, pParent);

		T& component = instance.AddComponent<T>();

		instance.SetName(component.GetName());

		if (p_focusOnCreation)
		{
			SelectActor(instance);
		}

		return instance;
	}
}