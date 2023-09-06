#include "UI_InputFloat.h"

UI::UI_InputFloat::UI_InputFloat(float pDefaultValue, float pStep, float pFastStep, const std::string& pLabel, const std::string& pFormat, bool pSelectAllOnClick)
	: UI_InputSingleScalar<float>(ImGuiDataType_Float, pDefaultValue, pStep, pFastStep, pLabel, pFormat, pSelectAllOnClick)
{
}
