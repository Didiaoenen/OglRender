#include "UI_DragInt.h"

UI::UI_DragInt::UI_DragInt(int pMin, int pMax, int pValue, float pSpeed, const std::string& pLabel, const std::string& pFormat) : 
	UI_DragSingleScalar<int>(ImGuiDataType_::ImGuiDataType_S32, pMin, pMax, pValue, pSpeed, pLabel, pFormat)
{
}
