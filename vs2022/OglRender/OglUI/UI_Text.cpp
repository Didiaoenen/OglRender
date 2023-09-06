#include "UI_Text.h"

UI::UI_Text::UI_Text(const std::string& pContent)
	: UI_DataWidget(mContent), mContent(pContent)
{
}

void UI::UI_Text::_Draw_Impl()
{
	ImGui::Text(mContent.c_str());
}
