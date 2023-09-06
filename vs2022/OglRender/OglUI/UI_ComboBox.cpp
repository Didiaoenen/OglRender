
#include <algorithm>

#include "UI_ComboBox.h"

UI::UI_ComboBox::UI_ComboBox(int pCurrentChoice)
	: UI_DataWidget<int>(mCurrentChoice), mCurrentChoice(pCurrentChoice)
{
}

void UI::UI_ComboBox::_Draw_Impl()
{
	if (mChoices.find(mCurrentChoice) == mChoices.end())
	{
		mCurrentChoice = mChoices.begin()->first;
	}

	if (ImGui::BeginCombo(mWidgetID.c_str(), mChoices[mCurrentChoice].c_str()))
	{
		for (const auto& [key, value] : mChoices)
		{
			bool selected = key == mCurrentChoice;

			if (ImGui::Selectable(value.c_str(), selected))
			{
				if (!selected)
				{
					ImGui::SetItemDefaultFocus();
					mCurrentChoice = key;
					mValueChangedEvent.Invoke(mCurrentChoice);
					this->NotifyChange();
				}
			}
		}

		ImGui::EndCombo();
	}
}
