#pragma once

#include "UI_SliderSingleScalar.h"

namespace UI
{
	class UI_SliderDouble : public UI_SliderSingleScalar<double>
	{
	public:
		UI_SliderDouble
		(
			double pMin = 0,
			double pMax = 100,
			double pValue = 50,
			ESliderOrientation pOrientation = ESliderOrientation::HORIZONTAL,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.6f"
		);
	};
}

