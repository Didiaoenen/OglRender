#pragma once

#include "UI_InputSingleScalar.h"

namespace UI
{
	class UI_InputDouble : public UI_InputSingleScalar<double>
	{
	public:
		UI_InputDouble
		(
			double pDefaultValue = 0.0,
			double pStep = 0.1,
			double pFastStep = 0.0,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.6f",
			bool pSelectAllOnClick = true
		);
	};
}

