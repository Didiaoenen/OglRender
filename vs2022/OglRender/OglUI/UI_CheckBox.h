#pragma once

#include "OglTools/Tools_Event.h"

#include "UI_DataWidget.h"

namespace UI
{
	class UI_CheckBox : public UI_DataWidget<bool>
	{
	public:
		UI_CheckBox(bool pValue = false, const std::string& pLabel = "");

	protected:
		void _Draw_Impl() override;

	public:
		bool mValue;
		std::string mLabel;
		Tools::Tools_Event<bool> mValueChangedEvent;
	};
}

