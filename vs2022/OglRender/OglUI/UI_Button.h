#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "UI_Color.h"
#include "UI_AButton.h"

namespace UI
{
class UI_Button : public UI_AButton
{
public:
	UI_Button(const std::string& pLabel = "", const glm::vec2& pSize = glm::zero<glm::vec2>(), bool pDisabled = false);

protected:
	void _Draw_Impl() override;

public:
	std::string mLabel;
	glm::vec2 mSize;
	bool mDisabled{ false };

	Color mIdleBackgroundColor;
	Color mHoveredBackgroundColor;
	Color mClickedBackgroundColor;

	Color mTextColor;
};
}

