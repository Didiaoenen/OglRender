
#include <OglUI/UI_DDTarget.h>

#include "Editor_EditorSettings.h"
#include "Editor_EditorRenderer.h"
#include "Editor_EditorActions.h"
#include "Editor_GameView.h"
#include "Editor_SceneView.h"

Editor::Editor_SceneView::Editor_SceneView(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings) :
	Editor_AViewControllable(p_title, p_opened, p_windowSettings, true), m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
}

void Editor::Editor_SceneView::Update(float p_deltaTime)
{
}

void Editor::Editor_SceneView::_Render_Impl()
{
}

void Editor::Editor_SceneView::RenderScene(uint8_t p_defaultRenderState)
{
}

void Editor::Editor_SceneView::RenderSceneForActorPicking()
{
}

void Editor::Editor_SceneView::HandleActorPicking()
{
}
