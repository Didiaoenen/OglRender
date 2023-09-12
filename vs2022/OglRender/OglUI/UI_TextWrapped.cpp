#include "UI_TextWrapped.h"

UI::UI_TextWrapped::UI_TextWrapped(const std::string& pContent) : 
	UI_Text(pContent)
{
}

void UI::UI_TextWrapped::_Draw_Impl()
{
	ImGui::TextWrapped(mContent.c_str());
}
