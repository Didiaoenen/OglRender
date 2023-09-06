#pragma once

#include "UI_InputSingleScalar.h"

namespace UI
{
	class UI_InputInt : public UI_InputSingleScalar<int>
	{
	public:
		UI_InputInt
		(
			int pDefaultValue = 0,
			int pStep = 1,
			int pFastStep = 0,
			const std::string& pLabel = "",
			const std::string& pFormat = "%d",
			bool pSelectAllOnClick = true
		);
	};
}

