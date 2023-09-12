#include "UI_SliderFloat.h"

UI::UI_SliderFloat::UI_SliderFloat(float pMin, float pMax, float pValue, ESliderOrientation pOrientation, const std::string& pLabel, const std::string& pFormat) : 
	UI_SliderSingleScalar<float>(ImGuiDataType_Float, pMin, pMax, pValue, pOrientation, pLabel, pFormat)
{
}
