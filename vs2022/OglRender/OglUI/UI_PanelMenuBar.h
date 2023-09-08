#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include <OglTools/Tools_Event.h>

#include "UI_APanel.h"
#include "UI_MenuList.h"

namespace UI
{
	class UI_PanelMenuBar : public UI_APanel
	{
	protected:
		void _Draw_Impl() override;
	};
}

