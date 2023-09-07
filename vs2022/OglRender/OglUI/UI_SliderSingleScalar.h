#pragma once

#include <glm/glm.hpp>

#include "OglTools/Tools_Event.h"

#include "UI_DataWidget.h"
#include "UI_ESliderOrientation.h"
#include "UI_Converter.h"

namespace UI
{
	template<typename T>
	class UI_SliderSingleScalar : public UI_DataWidget<T>
	{
		static_assert(std::is_scalar<T>::value, "Invalid SliderSingleScalar T (Scalar expected)");

	public:
		UI_SliderSingleScalar
		(
			ImGuiDataType pDataType,
			T pMin,
			T pMax,
			T pValue,
			ESliderOrientation pOrientation,
			const std::string& pLabel,
			const std::string& pFormat
		) : UI_DataWidget<T>(mValue), mDataType(pDataType), mMin(pMin), mMax(pMax), mValue(pValue), mOrientation(pOrientation), mLabel(pLabel), mFormat(pFormat) 
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

			bool valueChanged = false;

			switch (mOrientation)
			{
			case ESliderOrientation::HORIZONTAL:
				valueChanged = ImGui::SliderScalar((mLabel + this->mWidgetID).c_str(), mDataType, &mValue, &mMin, &mMax, mFormat.c_str());
				break;
			case ESliderOrientation::VERTICAL:
				valueChanged = ImGui::VSliderScalar((mLabel + this->mWidgetID).c_str(), UI_Converter::ToImVec2(mVerticalModeSize), mDataType, &mValue, &mMin, &mMax, mFormat.c_str());
				break;
			}

			if (valueChanged)
			{
				mValueChangedEvent.Invoke(mValue);
				this->NotifyChange();
			}
		}

	public:
		T mMin;
		T mMax;
		T mValue;
		ESliderOrientation mOrientation;
		glm::vec2 mVerticalModeSize;
		std::string mLabel;
		std::string mFormat;
		Tools::Tools_Event<T> mValueChangedEvent;

	private:
		ImGuiDataType mDataType;
	};
}

