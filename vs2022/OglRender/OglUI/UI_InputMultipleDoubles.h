#pragma once

#include "UI_InputMultipleScalars.h"

namespace UI
{
	template<size_t _Size>
	class UI_InputMultipleDoubles : public UI_InputMultipleScalars<double, _Size>
	{
	public:
		InputMultipleFloats
		(
			float pDefaultValue = 0.0f,
			float pStep = 0.1f,
			float pFastStep = 0.0f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.6f",
			bool pSelectAllOnClick = true
		) : UI_InputMultipleScalars<double, _Size>(ImGuiDataType_Double, pDefaultValue, pStep, pFastStep, pLabel, pFormat, pSelectAllOnClick)
		{
		}
	};
}

