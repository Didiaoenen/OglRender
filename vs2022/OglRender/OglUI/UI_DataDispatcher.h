#pragma once

#include <functional>

#include "UI_IPlugin.h"

#define TRY_GATHER(type, output)	if (auto plugin = GetPlugin<UI_DataDispatcher<type>>(); plugin) output = plugin->Gather();
#define TRY_PROVIDE(type, output)	if (auto plugin = GetPlugin<UI_DataDispatcher<type>>(); plugin) plugin->Provide(output);
#define TRY_NOTIFY_CHANGE(type)		if (auto plugin = GetPlugin<UI_DataDispatcher<type>>(); plugin) plugin->NotifyChange();

namespace UI
{
	template<typename T>
	class UI_DataDispatcher : public UI_IPlugin
	{
	public:
		void RegisterReference(T& pReference)
		{
			mDataPointer = &pReference;
		}

		void RegisterProvider(std::function<void(T)> pProvider)
		{
			mProvider = pProvider;
		}

		void RegisterGatherer(std::function<T(void)> pGatherer)
		{
			mGatherer = pGatherer;
		}

		void Provide(T pData)
		{
			if (mDataPointer)
			{
				*mDataPointer = pData;
			}
			else
			{
				mProvider(pData);
			}

			mValueChanged = false;
		}

		void NotifyChange()
		{
			mValueChanged = true;
		}

		T Gather()
		{
			return mDataPointer ? *mDataPointer : mGatherer();
		}

		virtual void Execute() override {}

	private:
		bool mValueChanged = false;
		T* mDataPointer = nullptr;
		std::function<void(T)> mProvider;
		std::function<T(void)> mGatherer;
	};
}