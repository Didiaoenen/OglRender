#pragma once

#include "UI_AWidget.h"

namespace UI
{
	class UI_Spacing : public UI_AWidget
	{
	public:
		UI_Spacing(uint16_t pSpaces = 1);

	protected:
		void _Draw_Impl() override;

	public:
		uint16_t mSpaces = 1;
	};
}

