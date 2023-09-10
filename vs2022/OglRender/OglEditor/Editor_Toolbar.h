#pragma once

#include <OglUI/UI_ButtonImage.h>
#include <OglUI/UI_PanelWindow.h>

namespace Editor
{
	class Editor_Toolbar : public UI::UI_PanelWindow
	{
	public:
		Editor_Toolbar
		(
			const std::string& p_title,
			bool p_opened,
			const UI::UI_PanelWindowSettings& p_windowSettings
		);

		void _Draw_Impl() override;

	private:
		UI::UI_ButtonImage* m_playButton;
		UI::UI_ButtonImage* m_pauseButton;
		UI::UI_ButtonImage* m_stopButton;
		UI::UI_ButtonImage* m_nextButton;
	};
}

