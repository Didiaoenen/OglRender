
#include <algorithm>

#include "UI_APanel.h"

uint64_t UI::UI_APanel::__PANEL_ID_INCREMENT = 0;

UI::UI_APanel::UI_APanel()
{
	mPanelID = "##" + std::to_string(__PANEL_ID_INCREMENT++);
}

void UI::UI_APanel::Draw()
{
	if (mEnabled)
	{
		_Draw_Impl();
	}
}

const std::string& UI::UI_APanel::GetPanelID() const
{
	return mPanelID;
}
