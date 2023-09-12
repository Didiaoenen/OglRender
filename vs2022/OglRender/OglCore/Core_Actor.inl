#pragma once

#include "Core_Actor.h"

namespace Core
{
	template<typename T, typename ...Args>
	inline T& Core_Actor::AddComponent(Args&& ...pArgs)
	{
		static_assert(std::is_base_of<Core_AComponent, T>::value, "T should derive from AComponent");

		if (auto found = GetComponent<T>(); !found)
		{
			mComponents.insert(mComponents.begin(), std::make_shared<T>(*this, pArgs...));
			T& instance = *dynamic_cast<T*>(mComponents.front().get());
			mComponentAddedEvent.Invoke(instance);
			if (mPlaying && IsActive())
			{
				reinterpret_cast<Core_AComponent&>(instance).OnAwake();
				reinterpret_cast<Core_AComponent&>(instance).OnEnable();
				reinterpret_cast<Core_AComponent&>(instance).OnStart();
			}
			return instance;
		}
		else
		{
			return *found;
		}
	}

	template<typename T>
	inline bool Core_Actor::RemoveComponent()
	{
		static_assert(std::is_base_of<Core_AComponent, T>::value, "T should derive from AComponent");
		static_assert(!std::is_same<Core_CTransform, T>::value, "You can't remove a CTransform from an actor");

		std::shared_ptr<T> result(nullptr);

		for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			result = std::dynamic_pointer_cast<T>(*it);
			if (result)
			{
				mComponentRemovedEvent.Invoke(*result.get());
				mComponents.erase(it);
				return true;
			}
		}

		return false;
	}

	template<typename T>
	inline T* Core_Actor::GetComponent()
	{
		static_assert(std::is_base_of<Core_AComponent, T>::value, "T should derive from AComponent");

		std::shared_ptr<T> result(nullptr);

		for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			result = std::dynamic_pointer_cast<T>(*it);
			if (result)
			{
				return result.get();
			}
		}

		return nullptr;
	}
}