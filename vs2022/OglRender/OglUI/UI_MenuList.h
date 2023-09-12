#pragma once

#include <vector>

#include <OglTools/Tools_Event.h>

#include "UI_Group.h"

namespace UI
{
	class UI_MenuList : public UI_Group
	{
	public:		
		UI_MenuList(const std::string& pName, bool pLocked = false);

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string mName;
		bool mLocked{ false };
		Tools::Tools_Event<> mClickedEvent;

	private:
		bool mOpened{ false };
	};
}

