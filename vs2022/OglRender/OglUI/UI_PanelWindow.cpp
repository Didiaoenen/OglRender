#include "UI_Converter.h"
#include "UI_PanelWindow.h"
#include "imgui_internal.h"

UI::UI_PanelWindow::UI_PanelWindow(const std::string& pName, bool pOpened, const UI::UI_PanelWindowSettings& pPanelSettings) :
	mName(pName),
	mResizable(pPanelSettings.resizable),
	mClosable(pPanelSettings.closable),
	mMovable(pPanelSettings.movable),
	mScrollable(pPanelSettings.scrollable),
	mDockable(pPanelSettings.dockable),
	mHideBackground(pPanelSettings.hideBackground),
	mForceHorizontalScrollbar(pPanelSettings.forceHorizontalScrollbar),
	mForceVerticalScrollbar(pPanelSettings.forceVerticalScrollbar),
	mAllowHorizontalScrollbar(pPanelSettings.allowHorizontalScrollbar),
	mBringToFrontOnFocus(pPanelSettings.bringToFrontOnFocus),
	mCollapsable(pPanelSettings.collapsable),
	mAllowInputs(pPanelSettings.allowInputs),
	mOpened(pOpened)
{
	mAutoSize = pPanelSettings.autoSize;
}

void UI::UI_PanelWindow::Open()
{
	if (!mOpened)
	{
		mOpened = true;
		mOpenEvent.Invoke();
	}
}

void UI::UI_PanelWindow::Close()
{
	if (mOpened)
	{
		mOpened = false;
		mCloseEvent.Invoke();
	}
}

void UI::UI_PanelWindow::Focus()
{
	ImGui::SetWindowFocus((mName + mPanelID).c_str());
}

void UI::UI_PanelWindow::SetOpened(bool pValue)
{
	if (pValue != mOpened)
	{
		mOpened = pValue;

		if (mOpened)
		{
			mOpenEvent.Invoke();
		}
		else
		{
			mCloseEvent.Invoke();
		}
	}
}

bool UI::UI_PanelWindow::IsOpened() const
{
	return mOpened;
}

bool UI::UI_PanelWindow::IsHovered() const
{
	return mHovered;
}

bool UI::UI_PanelWindow::IsFocused() const
{
	return mFocused;
}

bool UI::UI_PanelWindow::IsAppearing() const
{
	if (auto window = ImGui::FindWindowByName((mName + GetPanelID()).c_str()); window)
	{
		return window->Appearing;
	}
	else
	{
		return false;
	}
}

void UI::UI_PanelWindow::ScrollToBottom()
{
	mMustScrollToBottom = true;
}

void UI::UI_PanelWindow::ScrollToTop()
{
	mMustScrollToTop = true;
}

bool UI::UI_PanelWindow::IsScrolledToBottom() const
{
	return mScrolledToBottom;
}

bool UI::UI_PanelWindow::IsScrolledToTop() const
{
	return mScrolledToTop;
}

void UI::UI_PanelWindow::_Draw_Impl()
{
	if (mOpened)
	{
		int windowFlags = ImGuiWindowFlags_None;

		if (!mResizable)				windowFlags |= ImGuiWindowFlags_NoResize;
		if (!mMovable)					windowFlags |= ImGuiWindowFlags_NoMove;
		if (!mDockable)					windowFlags |= ImGuiWindowFlags_NoDocking;
		if (mHideBackground)			windowFlags |= ImGuiWindowFlags_NoBackground;
		if (mForceHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		if (mForceVerticalScrollbar)	windowFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
		if (mAllowHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
		if (!mBringToFrontOnFocus)		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (!mCollapsable)				windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (!mAllowInputs)				windowFlags |= ImGuiWindowFlags_NoInputs;
		if (!mScrollable)               windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
		if (!mTitleBar)					windowFlags |= ImGuiWindowFlags_NoTitleBar;

		ImVec2 minSizeConstraint = UI_Converter::ToImVec2(mMinSize);
		ImVec2 maxSizeConstraint = UI_Converter::ToImVec2(mMaxSize);

		if (minSizeConstraint.x <= 0.f || minSizeConstraint.y <= 0.f)
		{
			minSizeConstraint = { 0.0f, 0.0f };
		}

		if (maxSizeConstraint.x <= 0.f || maxSizeConstraint.y <= 0.f)
		{
			maxSizeConstraint = { 10000.f, 10000.f };
		}

		ImGui::SetNextWindowSizeConstraints(minSizeConstraint, maxSizeConstraint);

		if (ImGui::Begin((mName + mPanelID).c_str(), mClosable ? &mOpened : nullptr, windowFlags))
		{
			mHovered = ImGui::IsWindowHovered();
			mFocused = ImGui::IsWindowFocused();

			auto scrollY = ImGui::GetScrollY();

			mScrolledToBottom = scrollY == ImGui::GetScrollMaxY();
			mScrolledToTop = scrollY == 0.0f;

			if (!mOpened)
			{
				mCloseEvent.Invoke();
			}

			Update();

			if (mMustScrollToBottom)
			{
				ImGui::SetScrollY(ImGui::GetScrollMaxY());
				mMustScrollToBottom = false;
			}

			if (mMustScrollToTop)
			{
				ImGui::SetScrollY(0.0f);
				mMustScrollToTop = false;
			}

			DrawWidgets();
		}

		ImGui::End();
	}
}
