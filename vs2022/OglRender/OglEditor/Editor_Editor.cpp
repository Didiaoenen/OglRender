#include "Editor_MenuBar.h"
#include "Editor_Toolbar.h"
#include "Editor_Console.h"
#include "Editor_Profiler.h"
#include "Editor_GameView.h"
#include "Editor_Inspector.h"
#include "Editor_Hierarchy.h"
#include "Editor_SceneView.h"
#include "Editor_AssetView.h"
#include "Editor_AssetBrowser.h"
#include "Editor_HardwareInfo.h"
#include "Editor_MaterialEditor.h"
#include "Editor_ProjectSettings.h"
#include "Editor_AssetProperties.h"

#include "Editor_Editor.h"

Editor::Editor_Editor::Editor_Editor(Editor_Context& p_context) :
	m_context(p_context),
	m_editorRenderer(p_context),
	m_panelsManager(m_canvas),
	m_editorActions(m_context, m_editorRenderer, m_panelsManager)
{
}

Editor::Editor_Editor::~Editor_Editor()
{
}

void Editor::Editor_Editor::SetupUI()
{
}

void Editor::Editor_Editor::PreUpdate()
{
}

void Editor::Editor_Editor::Update(float p_deltaTime)
{
}

void Editor::Editor_Editor::HandleGlobalShortcuts()
{
}

void Editor::Editor_Editor::UpdateCurrentEditorMode(float p_deltaTime)
{
}

void Editor::Editor_Editor::UpdatePlayMode(float p_deltaTime)
{
}

void Editor::Editor_Editor::UpdateEditMode(float p_deltaTime)
{
}

void Editor::Editor_Editor::UpdateEditorPanels(float p_deltaTime)
{
}

void Editor::Editor_Editor::PrepareRendering(float p_deltaTime)
{
}

void Editor::Editor_Editor::RenderViews(float p_deltaTime)
{
}

void Editor::Editor_Editor::RenderEditorUI(float p_deltaTime)
{
}

void Editor::Editor_Editor::PostUpdate()
{
}
