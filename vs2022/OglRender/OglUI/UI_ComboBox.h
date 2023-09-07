#pragma once

#include <map>

#include "OglTools/Tools_Event.h"

#include "UI_DataWidget.h"

namespace UI
{
	class UI_ComboBox : public UI_DataWidget<int>
	{
	public:
		UI_ComboBox(int pCurrentChoice = 0);

	protected:
		void _Draw_Impl() override;

	public:
		std::map<int, std::string> mChoices;
		int mCurrentChoice;

	public:
		Tools::Tools_Event<int> mValueChangedEvent;
	};
}

