#include "UI_GroupCollapsable.h"

UI::UI_GroupCollapsable::UI_GroupCollapsable(const std::string& pName)
	: mName(pName)
{
}

void UI::UI_GroupCollapsable::_Draw_Impl()
{
	bool previouslyOpened = mOpened;

	if (ImGui::CollapsingHeader(mName.c_str(), mClosable ? &mOpened : nullptr))
	{
		UI_Group::_Draw_Impl();
	}

	if (mOpened != previouslyOpened)
	{
		if (mOpened)
		{
			mOpenEvent.Invoke();
		}
		else
		{
			mCloseEvent.Invoke();
		}
	}
}
