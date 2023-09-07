#include "UI_PanelMenuBar.h"

void UI::UI_PanelMenuBar::_Draw_Impl()
{
	if (!mWidgets.empty() && ImGui::BeginMainMenuBar())
	{
		DrawWidgets();
		ImGui::EndMainMenuBar();
	}
}
