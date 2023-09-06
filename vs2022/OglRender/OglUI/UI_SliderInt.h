#pragma once

#include "UI_SliderSingleScalar.h"

namespace UI
{
	class UI_SliderInt : public UI_SliderSingleScalar<int>
	{
	public:
		UI_SliderInt
		(
			int pMin = 0,
			int pMax = 100,
			int pValue = 50,
			ESliderOrientation pOrientation = ESliderOrientation::HORIZONTAL,
			const std::string& pLabel = "",
			const std::string& pFormat = "%d"
		);
	};
}

