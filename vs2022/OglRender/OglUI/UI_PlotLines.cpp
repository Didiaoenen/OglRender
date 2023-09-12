
#include "UI_Converter.h"
#include "UI_PlotLines.h"

UI::UI_PlotLines::UI_PlotLines(const std::vector<float>& pData, float pMinScale, float pMaxScale, const glm::vec2& pSize, const std::string& pOverlay, const std::string& pLabel, int pForceHover) : 
	UI_APlot(pData, pMinScale, pMaxScale, pSize, pOverlay, pLabel, pForceHover)
{
}

void UI::UI_PlotLines::_Draw_Impl()
{
	ImGui::PlotLines((mLabel + mWidgetID).c_str(), mData.data(), static_cast<int>(mData.size()), 0, mOverlay.c_str(), mMinScale, mMaxScale, UI_Converter::ToImVec2(mSize), sizeof(float), mForceHover);
}
