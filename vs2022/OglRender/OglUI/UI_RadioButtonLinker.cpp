#include <algorithm>

#include "UI_RadioButtonLinker.h"

UI::UI_RadioButtonLinker::UI_RadioButtonLinker()
	: UI_DataWidget<int>(mSelected)
{
}

void UI::UI_RadioButtonLinker::Link(UI_RadioButton& pRadioButton)
{
	pRadioButton.mRadioID = mAvailableRadioID++;
	auto listenerID = pRadioButton.mClickedEvent += std::bind(&UI_RadioButtonLinker::OnRadioButtonClicked, this, std::placeholders::_1);

	if (pRadioButton.IsSelected() && mSelected == -1)
	{
		mSelected = pRadioButton.mRadioID;
	}

	mRadioButtons.emplace_back(listenerID, std::ref(pRadioButton));
}

void UI::UI_RadioButtonLinker::Unlink(UI_RadioButton& pRadioButton)
{
	auto it = std::find_if(mRadioButtons.begin(), mRadioButtons.end(), [&pRadioButton](std::pair<Tools::ListenerID, std::reference_wrapper<UI_RadioButton>>& p_pair)
		{
			return &p_pair.second.get() == &pRadioButton;
		});

	if (it != mRadioButtons.end())
	{
		it->second.get().mClickedEvent.RemoveListener(it->first);
		mRadioButtons.erase(it);
	}
}

int UI::UI_RadioButtonLinker::GetSelected() const
{
	return mSelected;
}

void UI::UI_RadioButtonLinker::_Draw_Impl()
{

}

void UI::UI_RadioButtonLinker::OnRadioButtonClicked(int pRadioID)
{
	if (pRadioID != mSelected)
	{
		mSelected = pRadioID;
		mValueChangedEvent.Invoke(mSelected);
		this->NotifyChange();

		for (const auto& [listener, radioButton] : mRadioButtons)
		{
			radioButton.get().mSelected = radioButton.get().mRadioID == mSelected;
		}
	}
}
