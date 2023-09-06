#pragma once

#include "UI_DragMultipleScalars.h"

namespace UI
{
	template <size_t _Size>
	class UI_DragMultipleFloats : public UI_DragMultipleScalars<float, _Size>
	{
	public:
		UI_DragMultipleFloats
		(
			float pMin = 0,
			float pMax = 0,
			float pValue = 0,
			float pSpeed = 0.f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f"
		) : DragMultipleScalars<float, _Size>(ImGuiDataType_Float, pMin, pMax, pValue, pSpeed, pLabel, pFormat)
		{
		}
	};
}

