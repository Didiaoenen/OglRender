#pragma once

#include <memory>
#include <glm/glm.hpp>

#include <OglTools/Tools_Event.h>

#include "UI_APanelTransformable.h"
#include "UI_PanelWindowSettings.h"

namespace UI
{
	class UI_PanelWindow : public UI_APanelTransformable
	{
	public:
		UI_PanelWindow
		(
			const std::string& pName = "",
			bool pOpened = true,
			const PanelWindowSettings& pPanelSettings = PanelWindowSettings{}
		);

		void Open();

		void Close();

		void Focus();

		void SetOpened(bool pValue);

		bool IsOpened() const;

		bool IsHovered() const;

		bool IsFocused() const;

		bool IsAppearing() const;

		void ScrollToBottom();

		void ScrollToTop();

		bool IsScrolledToBottom() const;

		bool IsScrolledToTop() const;

	protected:
		void _Draw_Impl() override;

	public:
		std::string mName;

		glm::vec2 mMinSize { 0.f, 0.f };
		glm::vec2 mMaxSize { 0.f, 0.f };

		bool mResizable{ true };
		bool mClosable{ false };
		bool mMovable{ true };
		bool mScrollable{ true };
		bool mDockable{ false };
		bool mHideBackground{ false };
		bool mForceHorizontalScrollbar{ false };
		bool mForceVerticalScrollbar{ false };
		bool mAllowHorizontalScrollbar{ false };
		bool mBringToFrontOnFocus{ true };
		bool mCollapsable{ false };
		bool mAllowInputs{ true };
		bool mTitleBar{ true };

		Tools::Tools_Event<> mOpenEvent;
		Tools::Tools_Event<> mCloseEvent;

	private:
		bool mOpened{ false };
		bool mHovered{ false };
		bool mFocused{ false };
		bool mMustScrollToBottom{ false };
		bool mMustScrollToTop{ false };
		bool mScrolledToBottom{ false };
		bool mScrolledToTop{ false };
	};
}

