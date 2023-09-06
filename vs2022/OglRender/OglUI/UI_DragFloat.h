#pragma once

#include "UI_DragSingleScalar.h"

namespace UI
{
	class UI_DragFloat : public UI_DragSingleScalar<float>
	{
	public:
		UI_DragFloat
		(
			float pMin = 0,
			float pMax = 0,
			float pValue = 0,
			float pSpeed = 0.f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%d"
		);
	};
}

