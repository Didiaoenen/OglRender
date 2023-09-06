#pragma once

#include "UI_SliderSingleScalar.h"

namespace UI
{
	class UI_SliderFloat : public UI_SliderSingleScalar<float>
	{
	public:
		UI_SliderFloat
		(
			float pMin = 0,
			float pMax = 100,
			float pValue = 50,
			ESliderOrientation pOrientation = ESliderOrientation::HORIZONTAL,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f"
		);
	};
}

