#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "UI_Color.h"
#include "UI_AButton.h"

namespace UI
{
class UI_ButtonColored : public UI_AButton
{
public:
	UI_ButtonColored(const std::string& pLabel = "", const Color& pColor = {}, const glm::vec2& pSize = glm::zero<glm::vec2>(), bool pEnableAlpha = true);

protected:
	void _Draw_Impl() override;

public:
	std::string mLabel;
	Color mColor;
	glm::vec2 mSize;
	bool mEnableAlpha;
};
}

