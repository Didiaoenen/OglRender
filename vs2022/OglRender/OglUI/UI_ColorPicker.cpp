#include "UI_ColorPicker.h"

UI::UI_ColorPicker::UI_ColorPicker(bool pEnableAlpha, const Color& pDefaultColor)
	: UI_DataWidget<Color>(mColor), mEnableAlpha(pEnableAlpha), mColor(pDefaultColor)
{
}

void UI::UI_ColorPicker::_Draw_Impl()
{
	int flags = !mEnableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
	bool valueChanged = false;

	if (mEnableAlpha)
	{
		valueChanged = ImGui::ColorPicker4(mWidgetID.c_str(), &mColor.r, flags);
	}
	else
	{
		valueChanged = ImGui::ColorPicker3(mWidgetID.c_str(), &mColor.r, flags);
	}

	if (valueChanged)
	{
		mColorChangedEvent.Invoke(mColor);
		this->NotifyChange();
	}
}
