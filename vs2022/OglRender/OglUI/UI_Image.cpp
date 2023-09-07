#include "UI_Image.h"
#include "UI_Converter.h"

UI::UI_Image::UI_Image(uint32_t pTextureID, const glm::vec2& pSize)
	: mTextureID{ pTextureID }, mSize(pSize)
{
}

void UI::UI_Image::_Draw_Impl()
{
	ImGui::Image(mTextureID.mRaw, UI_Converter::ToImVec2(mSize), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
}
