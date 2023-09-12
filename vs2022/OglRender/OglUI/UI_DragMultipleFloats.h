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
			float pMin = 0.0f,
			float pMax = 1.0f,
			float pValue = 0.5f,
			float pSpeed = 0.1f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%.3f"
		) : DragMultipleScalars<float, _Size>(ImGuiDataType_Float, pMin, pMax, pValue, pSpeed, pLabel, pFormat)
		{
		}
	};
}

