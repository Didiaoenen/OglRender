#include "UI_SliderInt.h"

UI::UI_SliderInt::UI_SliderInt(int pMin, int pMax, int pValue, ESliderOrientation pOrientation, const std::string& pLabel, const std::string& pFormat)
	: UI_SliderSingleScalar<int>(ImGuiDataType_S32, pMin, pMax, pValue, pOrientation, pLabel, pFormat)
{
}
