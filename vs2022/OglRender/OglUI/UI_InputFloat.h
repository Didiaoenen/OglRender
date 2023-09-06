#pragma once

#include "UI_InputSingleScalar.h"

namespace UI
{
	class UI_InputFloat : public UI_InputSingleScalar<float>
	{
	public:
		UI_InputFloat
		(
			float pDefaultValue = 0,
			float pStep = 1,
			float pFastStep = 0,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f",
			bool pSelectAllOnClick = true
		);
	};
}

