#pragma once

#include "UI_APanelTransformable.h"

namespace UI
{
	class UI_PanelUndecorated : public UI_APanelTransformable
	{
	public:
		void _Draw_Impl() override;

	private:
		int CollectFlags();
	};
}

