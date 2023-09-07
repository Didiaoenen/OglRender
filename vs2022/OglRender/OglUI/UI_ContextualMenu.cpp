#include "UI_ContextualMenu.h"

void UI::UI_ContextualMenu::Execute()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}

void UI::UI_ContextualMenu::Close()
{
	ImGui::CloseCurrentPopup();
}
