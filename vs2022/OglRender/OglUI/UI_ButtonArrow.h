#pragma once

#include <string>

#include "UI_AButton.h"

namespace UI
{
	class UI_ButtonArrow : public UI_AButton
	{
	public:
		UI_ButtonArrow(ImGuiDir pDirection = ImGuiDir_None);

	protected:
		void _Draw_Impl() override;

	public:
		ImGuiDir mDirection;
	};
}

