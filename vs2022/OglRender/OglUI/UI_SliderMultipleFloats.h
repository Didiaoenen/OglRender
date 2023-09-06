#pragma once

#include "UI_SliderMultipleScalars.h"

namespace UI
{
	template<size_t _Size>
	class UI_SliderMultipleFloats : public UI_SliderMultipleScalars< float, _Size>
	{
	public:
		UI_SliderMultipleFloats
		(
			float pMin = 0.0f,
			float pMax = 1.0f,
			float pValue = 0.5f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f"
		) : UI_SliderMultipleScalars<float, _Size>(ImGuiDataType_Float, pMin, pMax, pValue, pLabel, pFormat)
		{
		}
	};
}

