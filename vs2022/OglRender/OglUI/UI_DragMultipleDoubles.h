#pragma once

#include "UI_DragMultipleScalars.h"

namespace UI
{
	template <size_t _Size>
	class UI_DragMultipleDoubles : public UI_DragMultipleScalars<double, _Size>
	{
	public:
		UI_DragMultipleDoubles
		(
			double pMin = 0.0,
			double pMax = 1.0,
			double pValue = 0.5,
			float pSpeed = 0.1f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.6f"
		) : DragMultipleScalars<float, _Size>(ImGuiDataType_Double, pMin, pMax, pValue, pSpeed, pLabel, pFormat)
		{
		}
	};
}

