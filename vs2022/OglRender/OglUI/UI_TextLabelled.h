#pragma once

#include "UI_Text.h"

namespace UI
{
	class UI_TextLabelled : public UI_Text
	{
	public:
		UI_TextLabelled(const std::string& pContent = "", const std::string& pLabel = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string mLabel;
	};
}

