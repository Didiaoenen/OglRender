#pragma once

#include "OglTools/Tools_Event.h"
#include "UI_DataWidget.h"

namespace UI
{
	class UI_MenuItem : public UI_DataWidget<bool>
	{
	public:
		UI_MenuItem(const std::string& pName, const std::string& pShortcut = "", bool pCheckable = false, bool pChecked = false);

	protected:
		void _Draw_Impl() override;

	public:
		std::string mName;
		std::string mShortcut;
		bool mCheckable;
		bool mChecked;
		Tools::Tools_Event<> mClickedEvent;
		Tools::Tools_Event<bool> mValueChangedEvent;

	private:
		bool m_selected;
	};
}

