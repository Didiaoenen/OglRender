#pragma once

#include <array>

#include <OglTools/Tools_Event.h>

#include "UI_DataWidget.h"

namespace UI
{
	template<typename T, size_t _Size>
	class UI_SliderMultipleScalars : public UI_DataWidget<std::array<T, _Size>>
	{
		static_assert(_Size > 1, "Invalid SliderMultipleScalars _Size (2 or more requiered)");

	public:
		UI_SliderMultipleScalars
		(
			ImGuiDataType_ pDataType,
			T pMin,
			T pMax,
			T pValue,
			const std::string& pLabel,
			const std::string& pFormat
		) : UI_DataWidget<std::array<T, _Size>>(mValues), mDataType(pDataType), mMin(pMin), mMax(pMax), mLabel(pLabel), mFormat(pFormat)
		{
			values.fill(pValue);
		}

	protected:
		void _Draw_Impl() override
		{
			if (mMax < mMin)
			{
				mMax = mMin;
			}

			for (size_t i = 0; i < _Size; ++i)
			{
				if (mValues[i] < mMin)
				{
					mValues[i] = mMin;
				}
				else if (mValues[i] > mMax)
				{
					mValues[i] = mMax;
				}
			}

			bool valueChanged = ImGui::SliderScalarN((mLabel + this->mWidgetID).c_str(), mDataType, mValues.data(), _Size, &mMin, &mMax, mFormat.c_str());

			if (valueChanged)
			{
				mValueChangedEvent.Invoke(mValues);
				this->NotifyChange();
			}
		}

	public:
		T mMin;
		T mMax;
		std::array<T, _Size> mValues;
		std::string mLabel;
		std::string mFormat;
		Tools::Tools_Event<std::array<T, _Size>&> mValueChangedEvent;

	protected:
		ImGuiDataType_ mDataType;
	};
}

