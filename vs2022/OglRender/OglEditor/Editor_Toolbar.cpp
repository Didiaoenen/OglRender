#include <OglUI/UI_Spacing.h>

#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_TextureManager.h>

#include "Editor_EditorActions.h"
#include "Editor_Toolbar.h"

Editor::Editor_Toolbar::Editor_Toolbar()
{
	std::string iconFolder = ":Textures/Icons/";

	auto& textureManager = Core::Core_ServiceLocator::Get<Core::Core_TextureManager>();

	glm::vec2 btnSize = {35, 25};
	m_playButton = &CreateWidget<UI::UI_Button>(ICON_MDI_PLAY, btnSize);
	m_pauseButton = &CreateWidget<UI::UI_Button>(ICON_MDI_PAUSE, btnSize);
	m_stopButton = &CreateWidget<UI::UI_Button>(ICON_MDI_STOP, btnSize);
	m_nextButton = &CreateWidget<UI::UI_Button>(ICON_MDI_STEP_FORWARD, btnSize);

	CreateWidget<UI::UI_Spacing>(0).mLineBreak = false;
	//auto& refreshButton = CreateWidget<UI::UI_Button>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Refresh")->id, btnSize);

	m_playButton->mLineBreak = false;
	m_pauseButton->mLineBreak = false;
	m_stopButton->mLineBreak = false;
	m_nextButton->mLineBreak = false;
	//refreshButton.mLineBreak = false;

	m_playButton->mClickedEvent += EDITOR_BIND(StartPlaying);
	m_pauseButton->mClickedEvent += EDITOR_BIND(PauseGame);
	m_stopButton->mClickedEvent += EDITOR_BIND(StopPlaying);
	m_nextButton->mClickedEvent += EDITOR_BIND(NextFrame);
	//refreshButton.mClickedEvent += EDITOR_BIND(RefreshScripts);

	EDITOR_EVENT(EditorModeChangedEvent) += [this](Editor::Editor_EditorActions::EEditorMode p_newMode)
		{
			auto enable = [](UI::UI_Button* p_button, bool p_enable)
				{
					p_button->mDisabled = !p_enable;
					//p_button->mTint = p_enable ? UI::Color{ 1.0f, 1.0f, 1.0f, 1.0f } : UI::Color{ 1.0f, 1.0f, 1.0f, 0.15f };
				};

			switch (p_newMode)
			{
			case Editor::Editor_EditorActions::EEditorMode::EDIT:
				enable(m_playButton, true);
				enable(m_pauseButton, false);
				enable(m_stopButton, false);
				enable(m_nextButton, false);
				break;
			case Editor::Editor_EditorActions::EEditorMode::PLAY:
				enable(m_playButton, false);
				enable(m_pauseButton, true);
				enable(m_stopButton, true);
				enable(m_nextButton, true);
				break;
			case Editor::Editor_EditorActions::EEditorMode::PAUSE:
				enable(m_playButton, true);
				enable(m_pauseButton, false);
				enable(m_stopButton, true);
				enable(m_nextButton, true);
				break;
			case Editor::Editor_EditorActions::EEditorMode::FRAME_BY_FRAME:
				enable(m_playButton, true);
				enable(m_pauseButton, false);
				enable(m_stopButton, true);
				enable(m_nextButton, true);
				break;
			}
		};

	EDITOR_EXEC(SetEditorMode(Editor::Editor_EditorActions::EEditorMode::EDIT));
}
