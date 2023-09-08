#pragma once

#include <OglTools/Tools_Event.h>

#include "UI_Text.h"

namespace UI
{
	class UI_TextClickable : public UI_Text
	{
	public:
		UI_TextClickable(const std::string& pContent = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		Tools::Tools_Event<> mClickedEvent;
		Tools::Tools_Event<> mDoubleClickedEvent;
	};
}

