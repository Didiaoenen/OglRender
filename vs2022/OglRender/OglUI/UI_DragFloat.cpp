#include "UI_DragFloat.h"

UI::UI_DragFloat::UI_DragFloat(float pMin, float pMax, float pValue, float pSpeed, const std::string& pLabel, const std::string& pFormat)
	: UI_DragSingleScalar<float>(ImGuiDataType_Float, pMin, pMax, pValue, pSpeed, pLabel, pFormat)
{
}
