#pragma once

#include <OglUI/UI_Button.h>
#include <OglUI/UI_ButtonImage.h>
#include <OglUI/UI_PanelSideBar.h>

namespace Editor
{
	class Editor_Toolbar : public UI::UI_PanelSideBar
	{
	public:
		Editor_Toolbar();

	private:
		UI::UI_Button* m_playButton;
		UI::UI_Button* m_pauseButton;
		UI::UI_Button* m_stopButton;
		UI::UI_Button* m_nextButton;
	};
}

