#pragma once

#include <glm/glm.hpp>

#include "UI_Color.h"
#include "UI_TextureID.h"
#include "UI_AButton.h"

namespace UI
{
class UI_ButtonImage : public UI_AButton
{
public:
	UI_ButtonImage(uint32_t pTextureID, const glm::vec2& pSize);

protected:
	void _Draw_Impl() override;

public:
	bool mDisabled{ false };

	Color mBackground{ 0, 0, 0, 0 };
	Color mTint{ 1, 1, 1, 1 };

	TextureID mTextureID;
	glm::vec2 mSize;
};
}

