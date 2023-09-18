#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include <OglTools/Tools_Event.h>

#include "UI_APanel.h"

namespace UI
{
	class UI_PanelSideBar : public UI_APanel
	{
	public:
	protected:
		void _Draw_Impl() override;
	};
}

