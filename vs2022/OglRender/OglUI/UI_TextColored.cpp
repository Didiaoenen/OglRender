#include "UI_TextColored.h"
#include "UI_Converter.h"

UI::UI_TextColored::UI_TextColored(const std::string& pContent, const Color& pColor)
	: UI_Text(pContent), mColor(pColor)
{
}

void UI::UI_TextColored::_Draw_Impl()
{
	ImGui::TextColored(UI_Converter::ToImVec4(mColor), mContent.c_str());
}
