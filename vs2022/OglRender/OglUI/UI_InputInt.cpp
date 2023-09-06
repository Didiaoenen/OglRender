#include "UI_InputInt.h"

UI::UI_InputInt::UI_InputInt(int pDefaultValue, int pStep, int pFastStep, const std::string& pLabel, const std::string& pFormat, bool pSelectAllOnClick)
	: UI_InputSingleScalar<int>(ImGuiDataType_S32, pDefaultValue, pStep, pFastStep, pLabel, pFormat, pSelectAllOnClick)
{
}
