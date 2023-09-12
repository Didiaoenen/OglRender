#include "UI_SliderDouble.h"

UI::UI_SliderDouble::UI_SliderDouble(double pMin, double pMax, double pValue, ESliderOrientation pOrientation, const std::string& pLabel, const std::string& pFormat) : 
	UI_SliderSingleScalar<double>(ImGuiDataType_Double, pMin, pMax, pValue, pOrientation, pLabel, pFormat)
{
}
