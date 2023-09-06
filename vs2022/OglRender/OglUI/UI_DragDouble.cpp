#include "UI_DragDouble.h"

UI::UI_DragDouble::UI_DragDouble(double pMin, double pMax, double pValue, float pSpeed, const std::string& pLabel, const std::string& pFormat)
	: UI_DragSingleScalar<double>(ImGuiDataType_Float, pMin, pMax, pValue, pSpeed, pLabel, pFormat)
{
}
