#pragma once

#include <glm/glm.hpp>

#include "UI_TextureID.h"
#include "UI_AWidget.h"

namespace UI
{
	class UI_Image : public UI_AWidget
	{
	public:
		UI_Image(uint32_t pTextureID, const glm::vec2& pSize);

	protected:
		void _Draw_Impl() override;

	public:
		TextureID mTextureID;
		glm::vec2 mSize;
	};
}

