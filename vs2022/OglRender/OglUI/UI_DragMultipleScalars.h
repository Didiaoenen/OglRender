#pragma once

#include <array>

#include "OglTools/Tools_Event.h"
#include "UI_DataWidget.h"

namespace UI
{
	template<typename T, size_t _Size>
	class UI_DragMultipleScalars : public UI_DataWidget<std::array<T, _Size>>
	{
		static_assert(_Size > 1, "Invalid DragMultipleScalars _Size (2 or more requiered");

	public:
		UI_DragMultipleScalars
		(
			ImGuiDataType_ pDataType,
			T pMin,
			T pMax,
			T pValue,
			float pSpeed,
			const std::string& pLabel,
			const std::string& pFormat
		) : UI_DataWidget<std::array<T, _Size>>(mValue), mDataType(pDataType), mMin(pMin), mMax(pMax), pValue(mValue), mSpeed(pSpeed), mLabel(pLabel), mFormat(pFormat)
		{
			mValues.fill(pValue);
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

			if (ImGui::DragScalarN((mLabel + this->mWidgetID).c_str(), mDataType, mValues.data(), _Size, mSpeed, &mMin, &mMax, mFormat.c_str()))
			{
				mValueChangedEvent.Invoke(mValues);
				this->NotifyChange();
			}
		}

	public:
		T mMin;
		T mMax;
		float mSpeed;
		std::array<T, _Size> mValues;
		std::string mLabel;
		std::string mFormat;
		Tools::Tools_Event<std::array<T, _Size>&> mValueChangeEvent;

	protected:
		ImGuiDataType_ m_dataType;
	};
}

