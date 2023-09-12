#pragma once

#include "UI_DragSingleScalar.h"

namespace UI
{
	class UI_DragDouble : public UI_DragSingleScalar<double>
	{
	public:
		UI_DragDouble
		(
			double pMin = 0.0,
			double pMax = 1.0,
			double pValue = 0.5,
			float pSpeed = 0.1f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.6f"
		);
	};
}

