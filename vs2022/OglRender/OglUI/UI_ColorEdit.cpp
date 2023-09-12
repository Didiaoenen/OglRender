#include "UI_ColorEdit.h"

UI::UI_ColorEdit::UI_ColorEdit(bool pEnableAlpha, const Color& pDefaultColor) : 
	UI_DataWidget<Color>(mColor), mEnableAlpha(pEnableAlpha), mColor(pDefaultColor)
{
}

void UI::UI_ColorEdit::_Draw_Impl()
{
	int flags = !mEnableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
	bool valueChanged = false;

	if (mEnableAlpha)
	{
		valueChanged = ImGui::ColorEdit4(mWidgetID.c_str(), &mColor.r, flags);
	}
	else
	{
		valueChanged = ImGui::ColorEdit3(mWidgetID.c_str(), &mColor.r, flags);
	}

	if (valueChanged)
	{
		mColorChangedEvent.Invoke(mColor);
		this->NotifyChange();
	}
}
