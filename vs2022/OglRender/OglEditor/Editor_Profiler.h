#pragma once

#include <OglUI/UI_Group.h>
#include <OglUI/UI_Button.h>
#include <OglUI/UI_Columns.h>
#include <OglUI/UI_CheckBox.h>
#include <OglUI/UI_PanelWindow.h>
#include <OglUI/UI_TextColored.h>

namespace Editor
{
	class Editor_Profiler : public UI::UI_PanelWindow
	{
	public:
		Editor_Profiler
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings,
			float p_frequency
		);

		void Update(float pDeltaTime);

		void Enable(bool pValue, bool p_disableLog = false);

	private:
		UI::Color CalculateActionColor(double p_percentage) const;
		//std::string GenerateActionString(OvAnalytics::Profiling::ProfilerReport::Action& p_action);

	private:
		enum class EProfilingMode
		{
			DEFAULT,
			CAPTURE
		};

		float m_frequency;
		float m_timer = 0.f;
		float m_fpsTimer = 0.f;
		EProfilingMode m_profilingMode = EProfilingMode::DEFAULT;

		UI::UI_AWidget* m_separator;
		UI::UI_Button* m_captureResumeButton;
		UI::UI_TextColored* m_fpsText;
		UI::UI_TextColored* m_elapsedFramesText;
		UI::UI_TextColored* m_elapsedTimeText;
		UI::UI_Columns<5>* m_actionList;
	};
}

