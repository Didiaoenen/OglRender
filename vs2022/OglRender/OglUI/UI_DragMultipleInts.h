#pragma once

#include "UI_DragMultipleScalars.h"

namespace UI
{
	template <size_t _Size>
	class UI_DragMultipleInts : public UI_DragMultipleScalars<int, _Size>
	{
	public:
		UI_DragMultipleInts
		(
			int pMin = 0,
			int pMax = 100,
			int pValue = 50,
			float pSpeed = 1.f,
			const std::string& pLabel = "",
			const std::string& pFormat = "%d"
		) : DragMultipleScalars<int, _Size>(ImGuiDataType_S32, pMin, pMax, pValue, pSpeed, pLabel, pFormat)
		{
		}
	};
}
