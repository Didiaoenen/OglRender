#include "UI_CheckBox.h"

UI::UI_CheckBox::UI_CheckBox(bool pValue, const std::string& pLabel) : 
	UI_DataWidget<bool>(mValue), mValue(pValue), mLabel(pLabel)
{
}

void UI::UI_CheckBox::_Draw_Impl()
{
	bool previousValue = mValue;

	ImGui::Checkbox((mLabel + mWidgetID).c_str(), &mValue);

	if (mValue != previousValue)
	{
		mValueChangedEvent.Invoke(mValue);
		this->NotifyChange();
	}
}
