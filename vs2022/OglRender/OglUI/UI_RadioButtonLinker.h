#pragma once

#include "UI_RadioButton.h"

namespace UI
{
	class UI_RadioButtonLinker : public UI_DataWidget<int>
	{
	public:
		UI_RadioButtonLinker();

		void Link(UI_RadioButton& pRadioButton);

		void Unlink(UI_RadioButton& pRadioButton);

		int GetSelected() const;

	protected:
		void _Draw_Impl() override;

	private:
		void OnRadioButtonClicked(int pRadioID);

	public:
		Tools::Tools_Event<int> mValueChangedEvent;

	private:
		int mAvailableRadioID{ 0 };
		int mSelected{ -1 };
		std::vector<std::pair<Tools::ListenerID, std::reference_wrapper<UI_RadioButton>>> mRadioButtons;
	};
}

