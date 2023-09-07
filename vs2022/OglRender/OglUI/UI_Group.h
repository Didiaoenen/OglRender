#pragma once

#include <vector>

#include "UI_WidgetContainer.h"

namespace UI
{
	class UI_Group : public UI_AWidget, public UI_WidgetContainer
	{
	protected:
		virtual void _Draw_Impl() override;
	};
}

