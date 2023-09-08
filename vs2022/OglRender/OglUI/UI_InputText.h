#pragma once

#include <OglTools/Tools_Event.h>

#include "UI_DataWidget.h"

namespace UI
{
	class UI_InputText : public UI_DataWidget<std::string>
	{
	public:
		UI_InputText(const std::string& pContent = "", const std::string& pLabel = "");
	
	protected:
		void _Draw_Impl() override;

	public:
		std::string mContent;
		std::string mLabel;
		bool mSelectAllOnClick = false;
		Tools::Tools_Event<std::string> mContentChangedEvent;
		Tools::Tools_Event<std::string> mEnterPressedEvent;
	};
}

