#include <algorithm>

#include <OglUI/UI_Button.h>
#include <OglUI/UI_Spacing.h>
#include <OglUI/UI_CheckBox.h>
#include <OglUI/UI_Separator.h>

#include "Editor_EditorActions.h"

#include "Editor_Console.h"

Editor::Editor_Console::Editor_Console(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings) :
	UI::UI_PanelWindow(pTitle, pOpened, pWindowSettings)
{
	SetIcon(ICON_MDI_VIEW_LIST " ");

	mAllowHorizontalScrollbar = true;

	auto& clearButton = CreateWidget<UI::UI_Button>("Clear");
	clearButton.mSize = { 50.f, 0.f };
	clearButton.mIdleBackgroundColor = { 0.5f, 0.f, 0.f, 1.f };
	clearButton.mClickedEvent += std::bind(&Editor_Console::Clear, this);
	clearButton.mLineBreak = false;

	auto& clearOnPlay = CreateWidget<UI::UI_CheckBox>(mClearOnPlay, "Auto clear on play");

	CreateWidget<UI::UI_Spacing>(5).mLineBreak = false;

	auto& enableDefault = CreateWidget<UI::UI_CheckBox>(true, "Default");
	auto& enableInfo = CreateWidget<UI::UI_CheckBox>(true, "Info");
	auto& enableWarning = CreateWidget<UI::UI_CheckBox>(true, "Warning");
	auto& enableError = CreateWidget<UI::UI_CheckBox>(true, "Error");

	clearOnPlay.mLineBreak = false;
	enableDefault.mLineBreak = false;
	enableInfo.mLineBreak = false;
	enableWarning.mLineBreak = false;
	enableError.mLineBreak = true;

	clearOnPlay.mValueChangedEvent += [this](bool p_value) { mClearOnPlay = p_value; };
	enableDefault.mValueChangedEvent += std::bind(&Editor_Console::SetShowDefaultLogs, this, std::placeholders::_1);
	enableInfo.mValueChangedEvent += std::bind(&Editor_Console::SetShowInfoLogs, this, std::placeholders::_1);
	enableWarning.mValueChangedEvent += std::bind(&Editor_Console::SetShowWarningLogs, this, std::placeholders::_1);
	enableError.mValueChangedEvent += std::bind(&Editor_Console::SetShowErrorLogs, this, std::placeholders::_1);

	CreateWidget<UI::UI_Separator>();

	mLogGroup = &CreateWidget<UI::UI_Group>();
	mLogGroup->ReverseDrawOrder();

	EDITOR_EVENT(PlayEvent) += std::bind(&Editor_Console::ClearOnPlay, this);

	//OvDebug::Logger::LogEvent += std::bind(&Console::OnLogIntercepted, this, std::placeholders::_1);
}

void Editor::Editor_Console::ClearOnPlay()
{
	if (mClearOnPlay)
	{
		Clear();
	}
}

void Editor::Editor_Console::Clear()
{
	//m_logTextWidgets.clear();
	mLogGroup->RemoveAllWidgets();
}

void Editor::Editor_Console::FilterLogs()
{
	//for (const auto& [widget, logLevel] : m_logTextWidgets)
	//{
	//	widget->enabled = IsAllowedByFilter(logLevel);
	//}
}

void Editor::Editor_Console::SetShowDefaultLogs(bool pValue)
{
	mShowDefaultLog = pValue;
	FilterLogs();
}

void Editor::Editor_Console::SetShowInfoLogs(bool pValue)
{
	mShowInfoLog = pValue;
	FilterLogs();
}

void Editor::Editor_Console::SetShowWarningLogs(bool pValue)
{
	mShowWarningLog = pValue;
	FilterLogs();
}

void Editor::Editor_Console::SetShowErrorLogs(bool pValue)
{
	mShowErrorLog = pValue;
	FilterLogs();
}
