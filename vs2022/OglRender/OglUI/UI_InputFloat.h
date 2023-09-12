#pragma once

#include "UI_InputSingleScalar.h"

namespace UI
{
	class UI_InputFloat : public UI_InputSingleScalar<float>
	{
	public:
		UI_InputFloat
		(
			float pDefaultValue = 0.0f,
			float pStep = 0.1f,
			float pFastStep = 0.0f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f",
			bool pSelectAllOnClick = true
		);
	};
}

