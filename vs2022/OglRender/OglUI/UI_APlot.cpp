#include "UI_APlot.h"

UI::UI_APlot::UI_APlot(const std::vector<float>& pData, float pMinScale, float pMaxScale, const glm::vec2& pSize, const std::string& pOverlay, const std::string& pLabel, int pForceHover)
	: UI_DataWidget(mData), mData(pData), mMinScale(pMinScale), mMaxScale(pMaxScale), mSize(pSize), mOverlay(pOverlay), mLabel(pLabel), mForceHover(pForceHover)
{
}
