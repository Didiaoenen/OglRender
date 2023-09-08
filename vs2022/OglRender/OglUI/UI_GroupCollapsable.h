#pragma once

#include <vector>

#include <OglTools/Tools_Event.h>

#include "UI_Group.h"

namespace UI
{
	class UI_GroupCollapsable : public UI_Group
	{
	public:
		UI_GroupCollapsable(const std::string& pName = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string mName;
		bool mClosable = false;
		bool mOpened = true;
		Tools::Tools_Event<> mCloseEvent;
		Tools::Tools_Event<> mOpenEvent;
	};
}

