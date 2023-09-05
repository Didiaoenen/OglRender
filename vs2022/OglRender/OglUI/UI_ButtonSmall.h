#pragma once

#include "UI_Color.h"
#include "UI_AButton.h"

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

