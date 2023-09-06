#include "UI_MenuItem.h"

UI::UI_MenuItem::UI_MenuItem(const std::string& pName, const std::string& pShortcut, bool pCheckable, bool pChecked)
	: UI_DataWidget(m_selected), mName(pName), mShortcut(pShortcut), mCheckable(pCheckable), mChecked(pChecked)
{
}

void UI::UI_MenuItem::_Draw_Impl()
{
	bool previousValue = mChecked;

	if (ImGui::MenuItem((mName + mWidgetID).c_str(), mShortcut.c_str(), mCheckable ? &mChecked : nullptr, mEnabled))
	{
		mClickedEvent.Invoke();
	}

	if (mChecked != previousValue)
	{
		mValueChangedEvent.Invoke(mCheckable);
		this->NotifyChange();
	}
}
