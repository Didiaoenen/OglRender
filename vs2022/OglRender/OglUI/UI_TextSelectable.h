#pragma once

#include "OglTools/Tools_Event.h"

#include "UI_Text.h"

namespace UI
{
	class UI_TextSelectable : public UI_Text
	{
	public:
		UI_TextSelectable(const std::string& pContent = "", bool pSelected = false, bool pDisabled = false);

	protected:
		virtual void _Draw_Impl() override;

	public:
		bool mSelected;
		bool mDisabled;

		Tools::Tools_Event<bool> mClickedEvent;
		Tools::Tools_Event<> mSelectedEvent;
		Tools::Tools_Event<> mUnselectedEvent;
	};
}

