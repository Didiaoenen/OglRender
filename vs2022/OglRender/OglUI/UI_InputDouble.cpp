#include "UI_InputDouble.h"

UI::UI_InputDouble::UI_InputDouble(double pDefaultValue, double pStep, double pFastStep, const std::string& pLabel, const std::string& pFormat, bool pSelectAllOnClick)
	: UI_InputSingleScalar<double>(ImGuiDataType_Double, pDefaultValue, pStep, pFastStep, pLabel, pFormat, pSelectAllOnClick)
{
}
