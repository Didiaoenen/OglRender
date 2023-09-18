#pragma once

#include <OglUI/UI_Group.h>
#include <OglUI/UI_PanelWindow.h>
#include <OglUI/UI_TextColored.h>

namespace Editor
{
	class Editor_Console : public UI::UI_PanelWindow
	{
	public:
		Editor_Console
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings
		);

		//void OnLogIntercepted(const OvDebug::LogData& p_logData);

		void ClearOnPlay();

		void Clear();

		void FilterLogs();

		//bool IsAllowedByFilter(OvDebug::ELogLevel p_logLevel);

	private:
		void SetShowDefaultLogs(bool pValue);
		void SetShowInfoLogs(bool pValue);
		void SetShowWarningLogs(bool pValue);
		void SetShowErrorLogs(bool pValue);

	private:
		UI::UI_Group* m_logGroup{ nullptr };
		//std::unordered_map<UI::UI_TextColored*, OvDebug::ELogLevel> m_logTextWidgets;

		bool m_clearOnPlay = true;
		bool m_showDefaultLog = true;
		bool m_showInfoLog = true;
		bool m_showWarningLog = true;
		bool m_showErrorLog = true;
	};
}

