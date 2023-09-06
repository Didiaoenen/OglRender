#include "UI_PlotHistogram.h"
#include "UI_Converter.h"

UI::UI_PlotHistogram::UI_PlotHistogram(const std::vector<float>& pData, float pMinScale, float pMaxScale, const glm::vec2& pSize, const std::string& pOverlay, const std::string& pLabel, int pForceHover)
	: UI_APlot(pData, pMinScale, pMaxScale, pSize, pOverlay, pLabel, pForceHover)
{
}

void UI::UI_PlotHistogram::_Draw_Impl()
{
	ImGui::PlotHistogram((mLabel + mWidgetID).c_str(), mData.data(), static_cast<int>(mData.size()), 0, mOverlay.c_str(), mMinScale, mMaxScale, UI_Converter::ToImVec2(mSize), sizeof(float), mForceHover);
}
