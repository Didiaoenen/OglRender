#include "UI_TextSelectable.h"

UI::UI_TextSelectable::UI_TextSelectable(const std::string& pContent, bool pSelected, bool pDisabled) : 
	UI_Text(pContent), mSelected(pSelected), mDisabled(pDisabled)
{
}

void UI::UI_TextSelectable::_Draw_Impl()
{
	if (ImGui::Selectable((mContent + mWidgetID).c_str(), &mSelected, mDisabled ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_None))
	{
		mClickedEvent.Invoke(mSelected);

		if (mSelected)
		{
			mSelectedEvent.Invoke();
		}
		else
		{
			mUnselectedEvent.Invoke();
		}
	}
}
