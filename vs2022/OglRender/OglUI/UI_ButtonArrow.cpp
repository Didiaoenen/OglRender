#include "UI_ButtonArrow.h"

UI::UI_ButtonArrow::UI_ButtonArrow(ImGuiDir pDirection) : 
	mDirection(pDirection)
{
}

void UI::UI_ButtonArrow::_Draw_Impl()
{
	if (ImGui::ArrowButton(mWidgetID.c_str(), mDirection))
	{
		mClickedEvent.Invoke();
	}
}
