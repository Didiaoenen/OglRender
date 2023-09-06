#pragma once

#include "UI_AWidget.h"

namespace UI
{
	class UI_Separator : public UI_AWidget
	{
	protected:
		void _Draw_Impl() override;
	};
}

