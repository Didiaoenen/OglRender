#pragma once

#include "UI_DataWidget.h"

namespace UI
{
	class UI_Text : public UI_DataWidget<std::string>
	{
	public:
		UI_Text(const std::string& pContent = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string mContent;
	};
}

