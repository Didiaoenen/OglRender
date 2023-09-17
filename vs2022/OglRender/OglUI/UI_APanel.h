#pragma once

#include <vector>
#include <unordered_map>

#include "UI_WidgetContainer.h"
#include "IconsMaterialDesignIcons.h"

namespace UI
{
	class UI_APanel : public UI_IDrawable, public UI_WidgetContainer
	{
	public:
		UI_APanel();

		void Draw() override;

		const std::string& GetPanelID() const;

	protected:
		virtual void _Draw_Impl() = 0;

	public:
		bool mEnabled = true;

	protected:
		std::string mPanelID;

	private:
		static uint64_t __PANEL_ID_INCREMENT;
	};
}

