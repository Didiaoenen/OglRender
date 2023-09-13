#include <OglUI/UI_Spacing.h>

#include <OglCore/Core_ServiceLocator.h>
#include <OglCore/Core_TextureManager.h>

#include "Editor_EditorActions.h"
#include "Editor_Toolbar.h"

Editor::Editor_Toolbar::Editor_Toolbar(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	UI::UI_PanelWindow(p_title, p_opened, p_windowSettings)
{
	std::string iconFolder = ":Textures/Icons/";

	auto& textureManager = Core::Core_ServiceLocator::Get<Core::Core_TextureManager>();

	m_playButton = &CreateWidget<UI::UI_ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Play")->id, glm::vec2 { 20, 20 });
	m_pauseButton = &CreateWidget<UI::UI_ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Pause")->id, glm::vec2 { 20, 20 });
	m_stopButton = &CreateWidget<UI::UI_ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Stop")->id, glm::vec2 { 20, 20 });
	m_nextButton = &CreateWidget<UI::UI_ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Next")->id, glm::vec2 { 20, 20 });

	CreateWidget<UI::UI_Spacing>(0).mLineBreak = false;
	auto& refreshButton = CreateWidget<UI::UI_ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Refresh")->id, glm::vec2 { 20, 20 });

	m_playButton->mLineBreak = false;
	m_pauseButton->mLineBreak = false;
	m_stopButton->mLineBreak = false;
	m_nextButton->mLineBreak = false;
	refreshButton.mLineBreak = false;

	m_playButton->mClickedEvent += EDITOR_BIND(StartPlaying);
	m_pauseButton->mClickedEvent += EDITOR_BIND(PauseGame);
	m_stopButton->mClickedEvent += EDITOR_BIND(StopPlaying);
	m_nextButton->mClickedEvent += EDITOR_BIND(NextFrame);
	refreshButton.mClickedEvent += EDITOR_BIND(RefreshScripts);

	EDITOR_EVENT(EditorModeChangedEvent) += [this](Editor::Editor_EditorActions::EEditorMode p_newMode)
		{
			auto enable = [](UI::UI_ButtonImage* p_button, bool p_enable)
				{
					p_button->mDisabled = !p_enable;
					p_button->mTint = p_enable ? UI::Color{ 1.0f, 1.0f, 1.0f, 1.0f } : UI::Color{ 1.0f, 1.0f, 1.0f, 0.15f };
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

void Editor::Editor_Toolbar::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

	UI::UI_PanelWindow::_Draw_Impl();

	ImGui::PopStyleVar();
}
