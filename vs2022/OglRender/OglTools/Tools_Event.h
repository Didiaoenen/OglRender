#pragma once

#include <functional>
#include <unordered_map>

namespace Tools
{
using ListenerID = uint64_t;

template<class... ArgTypes>
class Tools_Event
{
public:
	using Callback = std::function<void(ArgTypes...)>;

	ListenerID AddListener(Callback pCallback);

	ListenerID operator+=(Callback pCallback);

	bool RemoveListener(ListenerID pListenerID);

	bool operator-=(ListenerID pListenerID);

	void RemoveAllListeners();

	uint64_t GetListenerCount();

	void Invoke(ArgTypes... pArgs);

private:
	std::unordered_map<ListenerID, Callback> mCallbacks;
	ListenerID mAvailableListenerID{ 0 };
};
}