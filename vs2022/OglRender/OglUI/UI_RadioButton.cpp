#include "UI_RadioButton.h"

UI::UI_RadioButton::UI_RadioButton(bool pSelected, const std::string& pLabel)
	: UI_DataWidget<bool>(mSelected), mLabel(pLabel)
{
	if (pSelected)
	{
		Select();
	}
}

void UI::UI_RadioButton::Select()
{
	mSelected = true;
	mClickedEvent.Invoke(mRadioID);
}

bool UI::UI_RadioButton::IsSelected() const
{
	return mSelected;
}

bool UI::UI_RadioButton::GetRadioID() const
{
	return mRadioID;
}

void UI::UI_RadioButton::_Draw_Impl()
{
	if (ImGui::RadioButton((mLabel + mWidgetID).c_str(), mSelected))
	{
		mClickedEvent.Invoke(mRadioID);
		this->NotifyChange();
	}
}
