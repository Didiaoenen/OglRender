#pragma once

#include "Tools_Event.h"

namespace Tools
{
	template<class... ArgTypes>
	ListenerID Tools_Event<ArgTypes...>::AddListener(Callback pCallback)
	{
		ListenerID listenerID = mAvailableListenerID++;
		mCallbacks.emplace(listenerID, pCallback);
		return listenerID;
	}

	template<class... ArgTypes>
	ListenerID Tools_Event<ArgTypes...>::operator+=(Callback pCallback)
	{
		return AddListener(pCallback);
	}

	template<class... ArgTypes>
	bool Tools_Event<ArgTypes...>::RemoveListener(ListenerID pListenerID)
	{
		return mCallbacks.erase(pListenerID) != 0;
	}

	template<class... ArgTypes>
	bool Tools_Event<ArgTypes...>::operator-=(ListenerID pListenerID)
	{
		return RemoveListener(pListenerID);
	}

	template<class... ArgTypes>
	void Tools_Event<ArgTypes...>::RemoveAllListeners()
	{
		mCallbacks.clear();
	}

	template<class... ArgTypes>
	uint64_t Tools_Event<ArgTypes...>::GetListenerCount()
	{
		return mCallbacks.size();
	}

	template<class... ArgTypes>
	void Tools_Event<ArgTypes...>::Invoke(ArgTypes... pArgs)
	{
		for (auto const& [key, value] : mCallbacks)
		{
			value(pArgs...);
		}
	}
}