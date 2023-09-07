#pragma once

#include "UI_IPlugin.h"
#include "UI_WidgetContainer.h"
#include "UI_MenuList.h"
#include "UI_MenuItem.h"

namespace UI
{
	class UI_ContextualMenu : public UI_IPlugin, public UI_WidgetContainer
	{
	public:
		void Execute();

		void Close();
	};
}

