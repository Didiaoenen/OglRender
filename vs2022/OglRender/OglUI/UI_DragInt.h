#pragma once

#include "UI_DragSingleScalar.h"

namespace UI
{
	class UI_DragInt : public UI_DragSingleScalar<int>
	{
	public:
		UI_DragInt
		(
			int pMin = 0,
			int pMax = 0,
			int pValue = 0,
			float pSpeed = 0.f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%d"
		);
	};
}

