#pragma once

#include "OglTools/Tools_Event.h"
#include "UI_DataWidget.h"

namespace UI
{
	template<typename T>
	class UI_InputSingleScalar : public UI_DataWidget<T>
	{
		static_assert(std::is_scalar<T>::value, "Invalid InputSingleScalar T (Scalar expected)");

	public:
		UI_InputSingleScalar
		(
			ImGuiDataType pDataType,
			T pDefaultValue,
			T pStep,
			T pFastStep,
			const std::string& pLabel,
			const std::string& pFormat,
			bool pSelectAllOnClick
		) : UI_DataWidget<T>(mValue), mDataType(pDataType), mValue(pDefaultValue), mStep(pStep), mFastStep(pFastStep), mLabel(pLabel), mFormat(pFormat), mSelectAllOnClick(pSelectAllOnClick)
		{
		}

	protected:
		void _Draw_Impl() override
		{
			T previousValue = value;

			ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
			if (selectAllOnClick)
			{
				flags |= ImGuiInputTextFlags_AutoSelectAll;
			}

			bool enterPressed = ImGui::InputScalar((mLabel + this->mWidgetID).c_str(), mDataType, &mValue, mStep != 0.0f ? &mStep : nullptr, mFastStep != 0.0f ? &mFastStep : nullptr, mFormat.c_str(), flags);

			if (previousValue != mValue)
			{
				mContentChangedEvent.Invoke(mValue);
				this->NotifyChange();
			}

			if (enterPressed)
			{
				mEnterPressedEvent.Invoke(mValue);
			}
		}

	public:
		T mValue;
		T mStep;
		T mFastStep;
		std::string mLabel;
		std::string mFormat;
		bool mSelectAllOnClick;
		Tools::Tools_Event<T> mContentChangedEvent;
		Tools::Tools_Event<T> mEnterPressedEvent;

	private:
		ImGuiDataType mDataType;
	};
}

