#pragma once

#include "UI_Text.h"
#include "UI_Color.h"

namespace UI
{
	class UI_TextColored : public UI_Text
	{
	public:
		UI_TextColored(const std::string& pContent = "", const Color& pColor = Color(1.0f, 1.0f, 1.0f, 1.0f));

	public:
		Color mColor;

	protected:
		virtual void _Draw_Impl() override;
	};
}

