#pragma once

#include <vector>

#include "UI_AWidget.h"
#include "UI_EMemoryMode.h"

namespace UI
{
	class UI_WidgetContainer
	{
	public:
		void RemoveWidget(UI_AWidget& pWidget);

		void RemoveAllWidgets();

		void ConsiderWidget(UI_AWidget& pWidget, bool pManagerMemory = true);

		void UnconsiderWidget(UI_AWidget& pWidget);

		void CollectGarbages();

		void DrawWidgets();

		void ReverseDrawOrder(bool pReversed = true);

		template<typename T, typename ... Args>
		T& CreateWidget(Args&&... pArgs)
		{
			mWidgets.emplace_back(new T(pArgs...), UI_EMemoryMode::INTERNAL_MANAGMENT);
			T& instance = *reinterpret_cast<T*>(mWidgets.back().first);
			instance.SetParent(this);
			return instance;
		}

		std::vector<std::pair<UI_AWidget*, UI_EMemoryMode>>& GetWidgets();

	protected:
		std::vector<std::pair<UI_AWidget*, UI_EMemoryMode>> mWidgets;
		bool mReversedDrawOrder{ false };
	};
}

