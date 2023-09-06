#pragma once

#include "OglTools/Tools_Event.h"
#include "UI_DataWidget.h"

namespace UI
{
	class UI_RadioButtonLinker;

	class UI_RadioButton : public UI_DataWidget<bool>
	{
		friend UI_RadioButtonLinker;

	public:
		UI_RadioButton(bool pSelected = false, const std::string& pLabel = "");

		void Select();

		bool IsSelected() const;

		bool GetRadioID() const;

	protected:
		void _Draw_Impl() override;

	public:
		std::string mLabel;
		Tools::Tools_Event<int> mClickedEvent;

	private:
		bool mSelected{ false };
		int mRadioID{ 0 };
	};
}

