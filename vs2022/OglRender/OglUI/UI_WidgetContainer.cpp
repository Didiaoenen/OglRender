
#include <algorithm>

#include "UI_WidgetContainer.h"

void UI::UI_WidgetContainer::RemoveWidget(UI_AWidget& pWidget)
{
	auto found = std::find_if(mWidgets.begin(), mWidgets.end(), [&pWidget](std::pair<UI_AWidget*, UI_EMemoryMode>& pPair)
		{
			return pPair.first == &pWidget;
		});

	if (found != mWidgets.end())
	{
		if (found->second == UI_EMemoryMode::INTERNAL_MANAGMENT)
		{
			delete found->first;
			found->first = nullptr;
		}

		mWidgets.erase(found);
	}
}

void UI::UI_WidgetContainer::RemoveAllWidget()
{
	std::for_each(mWidgets.begin(), mWidgets.end(), [](std::pair<UI_AWidget*, UI_EMemoryMode>& pPair)
		{
			if (pPair.second == UI_EMemoryMode::INTERNAL_MANAGMENT)
			{
				delete pPair.first;
			}
		});

	mWidgets.clear();
}

void UI::UI_WidgetContainer::ConsiderWidget(UI_AWidget& pWidget, bool pManagerMemory)
{
	mWidgets.emplace_back(std::make_pair(&pWidget, pManagerMemory ? UI_EMemoryMode::INTERNAL_MANAGMENT : UI_EMemoryMode::EXTERNAL_MANAGMENT));
	pWidget.SetParent(this);
}

void UI::UI_WidgetContainer::UnconsiderWidget(UI_AWidget& pWidget)
{
	auto found = std::find_if(mWidgets.begin(), mWidgets.end(), [&pWidget](std::pair<UI_AWidget*, UI_EMemoryMode>& pPair)
		{
			return pPair.first == &pWidget;
		});

	if (found != mWidgets.end())
	{
		pWidget.SetParent(nullptr);
		mWidgets.erase(found);
	}
}

void UI::UI_WidgetContainer::CollectGarbages()
{
	mWidgets.erase(std::remove_if(mWidgets.begin(), mWidgets.end(), [](std::pair<UI_AWidget*, UI_EMemoryMode>& pItem)
		{
			bool toDestroy = pItem.first && pItem.first->IsDestroyed();

			if (toDestroy && pItem.second == UI_EMemoryMode::INTERNAL_MANAGMENT)
			{
				delete pItem.first;
				pItem.first = nullptr;
			}

			return toDestroy;
		}), mWidgets.end());
}

void UI::UI_WidgetContainer::DrawWidgets()
{
	CollectGarbages();
	if (mReversedDrawOrder)
	{
		for (auto it = mWidgets.crbegin(); it != mWidgets.crend(); ++it)
		{
			it->first->Draw();
		}
	}
	else
	{
		for (const auto& width : mWidgets)
		{
			width.first->Draw();
		}
	}
}

void UI::UI_WidgetContainer::ReverseDrawOrder(bool pReversed)
{
	mReversedDrawOrder = pReversed;
}

std::vector<std::pair<UI::UI_AWidget*, UI::UI_EMemoryMode>>& UI::UI_WidgetContainer::GetWidgets()
{
	return mWidgets;
}
