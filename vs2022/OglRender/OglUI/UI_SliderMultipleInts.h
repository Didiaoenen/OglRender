#pragma once

#include "UI_SliderMultipleScalars.h"

namespace UI
{
	template<size_t _Size>
	class UI_SliderMultipleInts : public UI_SliderMultipleScalars<int, _Size>
	{
	public:
		SliderMultipleInts
		(
			int pMin = 0,
			int pMax = 100,
			int pValue = 50,
			const std::string& pLabel = "",
			const std::string& pFormat = "%d"
		) : UI_SliderMultipleScalars<int, _Size>(ImGuiDataType_S32, pMin, pMax, pValue, pLabel, pFormat) 
		{
		}
	};
}

