#include "UI_TextClickable.h"

UI::UI_TextClickable::UI_TextClickable(const std::string& pContent) : 
    UI_Text(pContent)
{
}

void UI::UI_TextClickable::_Draw_Impl()
{
    bool useless = false;

    if (ImGui::Selectable((mContent + mWidgetID).c_str(), &useless, ImGuiSelectableFlags_AllowDoubleClick))
    {
        if (ImGui::IsMouseDoubleClicked(0))
        {
            mDoubleClickedEvent.Invoke();
        }
        else
        {
            mClickedEvent.Invoke();
        }
    }
}
