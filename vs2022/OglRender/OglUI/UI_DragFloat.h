#pragma once

#include "UI_DragSingleScalar.h"

namespace UI
{
	class UI_DragFloat : public UI_DragSingleScalar<float>
	{
	public:
		UI_DragFloat
		(
			float pMin = 0.f,
			float pMax = 1.f,
			float pValue = 0.5f,
			float pSpeed = 0.1f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f"
		);
	};
}

