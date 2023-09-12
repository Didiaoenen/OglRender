
#include "UI_Converter.h"
#include "UI_Dummy.h"

UI::UI_Dummy::UI_Dummy(const glm::vec2& pSize) : 
	mSize(pSize)
{
}

void UI::UI_Dummy::_Draw_Impl()
{
	ImGui::Dummy(UI_Converter::ToImVec2(mSize));
}
