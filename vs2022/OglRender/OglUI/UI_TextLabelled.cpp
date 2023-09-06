#include "UI_TextLabelled.h"

UI::UI_TextLabelled::UI_TextLabelled(const std::string& pContent, const std::string& pLabel)
	: UI_Text(pContent), mLabel(pLabel)
{
}

void UI::UI_TextLabelled::_Draw_Impl()
{
	ImGui::LabelText((mLabel + mWidgetID).c_str(), mContent.c_str());
}
