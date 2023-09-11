#include "UI_AWidget.h"

uint64_t UI::UI_AWidget::__WIDGET_ID_INCREMENT = 0;

UI::UI_AWidget::UI_AWidget()
{
	mWidgetID = "##" + std::to_string(__WIDGET_ID_INCREMENT++);
}

void UI::UI_AWidget::Draw()
{
	if (mEnabled)
	{
		_Draw_Impl();

		if (mAutoExecutePlugins)
		{
			ExecutePlugins();
		}

		if (!mLineBreak)
		{
			ImGui::SameLine();
		}
	}
}

void UI::UI_AWidget::LinkTo(const UI_AWidget& pWidget)
{
	mWidgetID = pWidget.mWidgetID;
}

void UI::UI_AWidget::Destroy()
{
	mDestroyed = true;
}

bool UI::UI_AWidget::IsDestroyed() const
{
	return mDestroyed;
}

void UI::UI_AWidget::SetParent(UI_WidgetContainer* pParent)
{
	mParent = pParent;
}

bool UI::UI_AWidget::HasParent() const
{
	return mParent;
}

UI::UI_WidgetContainer* UI::UI_AWidget::GetParent()
{
	return mParent;
}
