#include "UI_MenuList.h"

UI::UI_MenuList::UI_MenuList(const std::string& pName, bool pLocked)
	: mName(pName), mLocked(pLocked)
{
}

void UI::UI_MenuList::_Draw_Impl()
{
	if (ImGui::BeginMenu(mName.c_str(), !mLocked))
	{
		if (!mOpened)
		{
			mClickedEvent.Invoke();
			mOpened = true;
		}

		DrawWidgets();
		ImGui::EndMenu();
	}
	else
	{
		mOpened = false;
	}
}
