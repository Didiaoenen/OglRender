#pragma once

#include <string>

#include "OglTools/Tools_Event.h"

#include "UI_AWidget.h"

namespace UI
{
	class UI_AButton : public UI_AWidget
	{
	protected:
		void _Draw_Impl() override = 0;

	public:
		Tools::Tools_Event<> mClickedEvent;
	};
}