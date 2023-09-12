#pragma once

#include "UI_SliderSingleScalar.h"

namespace UI
{
	class UI_SliderFloat : public UI_SliderSingleScalar<float>
	{
	public:
		UI_SliderFloat
		(
			float pMin = 0.0f,
			float pMax = 1.0f,
			float pValue = 0.5f,
			ESliderOrientation pOrientation = ESliderOrientation::HORIZONTAL,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f"
		);
	};
}

