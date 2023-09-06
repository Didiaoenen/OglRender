#pragma once

#include "UI_InputMultipleScalars.h"

namespace UI
{
	template<size_t _Size>
	class UI_InputMultipleFloats : public UI_InputMultipleScalars<float, _Size>
	{
	public:
		InputMultipleFloats
		(
			float pDefaultValue = 0.0f,
			float pStep = 0.1f,
			float pFastStep = 0.0f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f",
			bool pSelectAllOnClick = true
		) : UI_InputMultipleScalars<float, _Size>(ImGuiDataType_Float, pDefaultValue, pStep, pFastStep, pLabel, pFormat, pSelectAllOnClick)
		{
		}
	};
}

