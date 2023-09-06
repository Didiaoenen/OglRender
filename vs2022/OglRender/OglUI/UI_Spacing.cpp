#include "UI_Spacing.h"

UI::UI_Spacing::UI_Spacing(uint16_t pSpaces)
	: mSpaces(pSpaces)
{
}

void UI::UI_Spacing::_Draw_Impl()
{
	for (uint16_t i = 0; i < mSpaces; ++i)
	{
		ImGui::Spacing();

		if (i + 1 < mSpaces)
		{
			ImGui::SameLine();
		}
	}
}
