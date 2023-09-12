
#include "UI_Converter.h"
#include "UI_ProgressBar.h"

UI::UI_ProgressBar::UI_ProgressBar(float pFraction, const glm::vec2& pSize, const std::string& pOverlay) : 
	mFraction(pFraction), mSize(pSize), mOverlay(pOverlay)
{
}

void UI::UI_ProgressBar::_Draw_Impl()
{
	ImGui::ProgressBar(mFraction, UI_Converter::ToImVec2(mSize), !mOverlay.empty() ? mOverlay.c_str() : nullptr);
}
