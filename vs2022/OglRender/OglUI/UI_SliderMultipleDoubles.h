#pragma once

#include "UI_SliderMultipleScalars.h"

namespace UI
{
	template<size_t _Size>
	class UI_SliderMultipleDoubles : public UI_SliderMultipleScalars<double, _Size>
	{
	public:
		UI_SliderMultipleDoubles
		(
			double pMin = 0.0f,
			double pMax = 1.0f,
			double pValue = 0.5f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.6f"
		) : UI_SliderMultipleScalars<double, _Size>(ImGuiDataType_Double, pMin, pMax, pValue, pLabel, pFormat)
		{
		}
	};
}

