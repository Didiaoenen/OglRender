#include <OglCore/Core_CCamera.h>

#include "Editor_EditorActions.h"
#include "Editor_EditorSettings.h"
#include "Editor_EditorRenderer.h"
#include "Editor_GameView.h"

Editor::Editor_GameView::Editor_GameView(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	Editor_AView(p_title, p_opened, p_windowSettings), m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
}

void Editor::Editor_GameView::Update(float p_deltaTime)
{
}

void Editor::Editor_GameView::_Render_Impl()
{
}

bool Editor::Editor_GameView::HasCamera() const
{
	return false;
}

std::optional<Render::Render_Frustum> Editor::Editor_GameView::GetActiveFrustum() const
{
	return std::optional<Render::Render_Frustum>();
}
