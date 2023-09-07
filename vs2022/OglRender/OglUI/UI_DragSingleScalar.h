#pragma once

#include "OglTools/Tools_Event.h"

#include "UI_DataWidget.h"

namespace UI
{
	template<typename T>
	class UI_DragSingleScalar : public UI_DataWidget<T>
	{
		static_assert(std::is_scalar<T>::value, "Tnvalid DragSingleScalar T (Scalar expected)");
	
	public:
		UI_DragSingleScalar
		(
			ImGuiDataType pDataType,
			T pMin,
			T pMax,
			T pValue,
			float pSpeed,
			const std::string& pLabel,
			const std::string& pFormat
		) : UI_DataWidget<T>(mValue), mDataType(pDataType), mMin(pMin), mMax(pMax), mValue(pValue), mSpeed(pSpeed), mLabel(pLabel), mFormat(pFormat)
		{
		}

	protected:
		void _Draw_Impl() override
		{
			if (mMax < mMin)
			{
				mMax = mMin;
			}

			if (mValue < mMin)
			{
				mValue = mMin;
			}
			else if (mValue > mMax)
			{
				mValue = mMax;
			}

			if (ImGui::DragScalar((mLabel + this->mWidgetID).c_str(), mDataType, &mValue, mSpeed, &mMin, &mMax, mFormat.c_str()))
			{
				mValueChangeEvent.Invoke(mValue);
				this->NotifyChange();
			}
		}

	public:
		T mMin;
		T mMax;
		T mValue;
		float mSpeed;
		std::string mLabel;
		std::string mFormat;
		Tools::Tools_Event<T> mValueChangeEvent; 

	private:
		ImGuiDataType mDataType;
	};
}

