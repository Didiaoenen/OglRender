#pragma once

#include <array>

#include "OglTools/Tools_Event.h"

#include "UI_DataWidget.h"

namespace UI
{
	template <typename T, size_t _Size>
	class UI_InputMultipleScalars : public UI_DataWidget<std::array<T, _Size>>
	{
		static_assert(_Size > 1, "Invalid InputMultipleScalars _Size (2 or more requiered)");
		static_assert(std::is_scalar<T>::value, "Invalid InputMultipleScalars T (Scalar expected)");

	public:
		UI_InputMultipleScalars
		(
			ImGuiDataType pDataType,
			T pDefaultValue,
			T pStep,
			T pFastStep,
			const std::string& pLabel,
			const std::string& pFormat,
			bool pSelectAllOnClick
		) : UI_DataWidget<std::array<T, _Size>>(mValues), mDataType(pDataType), mStep(pStep), mFastStep(pFastStep), mLabel(pLabel), mFormat(pFormat), mSelectAllOnClick(pSelectAllOnClick)
		{
			values.fill(p_defaultValue);
		}

	protected:
		void _Draw_Impl() override
		{
			std::array<T, _Size> previousValue = mValues;

			ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
			if (mSelectAllOnClick)
			{
				flags |= ImGuiInputTextFlags_AutoSelectAll;
			}

			bool enterPressed = ImGui::InputScalarN((mLabel + mWidgetID).c_str(), mDataType, mValues.data(), _Size, mStep != 0.0f ? &mStep : nullptr, fastStep != 0.0f ? &mFastStep : nullptr, mFormat.c_str(), flags);

			bool hasChanged = false;

			for (size_t i = 0; i < _Size; ++i)
			{
				if (previousValue[i] != mValues[i])
				{
					hasChanged = true;
				}
			}

			if (hasChanged)
			{
				mContentChangedEvent.Invoke(mValues);
				this->NotifyChange();
			}

			if (enterPressed)
			{
				mEnterPressedEvent.Invoke(mValues);
			}
		}

	public:
		std::array<T, _Size> mValues;
		T mStep;
		T mFastStep;
		std::string mLabel;
		std::string mFormat;
		bool mSelectAllOnClick;
		Tools::Tools_Event<std::array<T, _Size>&> mContentChangedEvent;
		Tools::Tools_Event<std::array<T, _Size>&> mEnterPressedEvent;

	private:
		ImGuiDataType mDataType;
	};
}

