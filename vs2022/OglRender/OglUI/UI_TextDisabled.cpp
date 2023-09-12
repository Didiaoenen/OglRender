#include "UI_TextDisabled.h"

UI::UI_TextDisabled::UI_TextDisabled(const std::string& pContent) : 
	UI_Text(pContent)
{
}

void UI::UI_TextDisabled::_Draw_Impl()
{
	ImGui::TextDisabled(mContent.c_str());
}
