#pragma once

#include <string>

#include "UI_AButton.h"
#include "UI_Color.h"

namespace UI
{
	class UI_ButtonSmall : public UI_AButton
	{
	public:
		UI_ButtonSmall(const std::string& pLabel = "");

	protected:
		void _Draw_Impl() override;

	public:
		std::string mLabel;

		Color mIdleBackgroundColor;
		Color mHoveredBackgroundColor;
		Color mClickedBackgroundColor;

		Color mTextColor;
	};
}

