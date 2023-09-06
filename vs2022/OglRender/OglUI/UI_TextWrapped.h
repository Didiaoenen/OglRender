#pragma once

#include "UI_Text.h"

namespace UI
{
	class UI_TextWrapped : public UI_Text
	{
	public:
		UI_TextWrapped(const std::string& pContent = "");

	protected:
		virtual void _Draw_Impl() override;
	};
}

