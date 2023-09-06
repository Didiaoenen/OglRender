#pragma once

#include "UI_InputMultipleScalars.h"

namespace UI
{
	template <size_t _Size>
	class UI_InputMultipleInts : public UI_InputMultipleScalars<int, _Size>
	{
	public:
		UI_InputMultipleInts
		(
			int pDefaultValue = 0,
			int pStep = 1,
			int pFastStep = 0,
			const std::string& pLabel = "",
			const std::string& pFormat = "%d",
			bool pSelectAllOnClick = true
		) : UI_InputMultipleScalars<int, _Size>(ImGuiDataType_S32, pDefaultValue, pStep, pFastStep, pLabel, pFormat, pSelectAllOnClick)
		{
		}
	};
}

