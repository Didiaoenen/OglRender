#pragma once

#include "OglTools/Tools_Event.h"
#include "UI_WidgetContainer.h"
#include "UI_DataWidget.h"

namespace UI
{
	class UI_TreeNode : public UI_DataWidget<std::string>, public UI_WidgetContainer
	{
	public:
		UI_TreeNode(const std::string& pName = "", bool pArrowClickToOpen = false);

		void Open();

		void Close();

		bool IsOpened() const;

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string mName;
		bool mSelected = false;
		bool mLeaf = false;

		Tools::Tools_Event<> mClickedEvent;
		Tools::Tools_Event<> mDoubleClickedEvent;
		Tools::Tools_Event<> mOpenedEvent;
		Tools::Tools_Event<> mClosedEvent;

	private:
		bool mArrowClickToOpen{ false };
		bool mShouldOpen{ false };
		bool mShouldClose{ false };
		bool mOpened{ false };
	};
}
