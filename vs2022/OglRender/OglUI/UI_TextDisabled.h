#pragma once

#include "UI_Text.h"

namespace UI
{
	class UI_TextDisabled : public UI_Text
	{
	public:
		UI_TextDisabled(const std::string& pContent = "");

	protected:
		virtual void _Draw_Impl() override;
	};
}

