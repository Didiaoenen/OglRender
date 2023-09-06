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
			float pMin = 0.0f,
			float pMax = 1.0f,
			float pValue = 0.5f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.6f"
		) : UI_SliderMultipleScalars<float, _Size>(ImGuiDataType_Double, pMin, pMax, pValue, pLabel, pFormat)
		{
		}
	};
}

