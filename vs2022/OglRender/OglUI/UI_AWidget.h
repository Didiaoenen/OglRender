#pragma once

#include <string>

#include "UI_IDrawable.h"
#include "UI_Pluginable.h"
#include "UI_DataDispatcher.h"
#include "imgui.h"

namespace UI
{
	class UI_WidgetContainer;

	class UI_AWidget : public UI_IDrawable, public UI_Pluginable
	{
	public:
		UI_AWidget();

		virtual void Draw() override;

		void LinkTo(const UI_AWidget& pWidget);

		void Destroy();

		bool IsDestroyed() const;

		void SetParent(UI_WidgetContainer* pParent);

		bool HasParent() const;

		UI_WidgetContainer* GetParent();

	protected:
		virtual void _Draw_Impl() = 0;

	public:
		bool mEnabled{ true };
		bool mLineBreak{ true };

	protected:
		UI_WidgetContainer* mParent{ nullptr };
		std::string mWidgetID{ std::string("?") };
		bool mAutoExecutePlugins{ true };

	private:
		static uint64_t __WIDGET_ID_INCREMENT;
		bool mDestroyed{ false };
	};
}

