#include "UI_InputText.h"

UI::UI_InputText::UI_InputText(const std::string& pContent, const std::string& pLabel)
	: UI_DataWidget<std::string>(mContent), mContent(pContent), mLabel(pLabel)
{
}

void UI::UI_InputText::_Draw_Impl()
{
	std::string previousContent = mContent;

	mContent.resize(256, '\0');
	bool enterPressed = ImGui::InputText((mLabel + mWidgetID).c_str(), &mContent[0], 256, ImGuiInputTextFlags_EnterReturnsTrue | (mSelectAllOnClick ? ImGuiInputTextFlags_AutoSelectAll : 0));
	mContent = mContent.c_str();

	if (mContent != previousContent)
	{
		mContentChangedEvent.Invoke(mContent);
		this->NotifyChange();
	}

	if (enterPressed)
	{
		mEnterPressedEvent.Invoke(mContent);
	}
}
