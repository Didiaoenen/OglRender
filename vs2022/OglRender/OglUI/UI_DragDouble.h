#pragma once

#include "UI_DragSingleScalar.h"

namespace UI
{
	class UI_DragDouble : public UI_DragSingleScalar<double>
	{
	public:
		UI_DragDouble
		(
			double pMin = 0,
			double pMax = 0,
			double pValue = 0,
			float pSpeed = 0.f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%d"
		);
	};
}

