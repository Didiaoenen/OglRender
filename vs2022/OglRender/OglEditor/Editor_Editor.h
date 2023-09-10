#pragma once

#include <OglRenderer/Render_Camera.h>

#include <OglUI/UI_Canvas.h>

#include "Editor_EditorRenderer.h"
#include "Editor_EditorActions.h"
#include "Editor_PanelsManager.h"

#include "Editor_Context.h"

namespace Editor 
{
	class Editor_Editor
	{
	public:
		Editor_Editor(Editor_Context& p_context);

		~Editor_Editor();

		void SetupUI();

		void PreUpdate();

		void Update(float p_deltaTime);

		void HandleGlobalShortcuts();

		void UpdateCurrentEditorMode(float p_deltaTime);

		void UpdatePlayMode(float p_deltaTime);

		void UpdateEditMode(float p_deltaTime);

		void UpdateEditorPanels(float p_deltaTime);

		void PrepareRendering(float p_deltaTime);

		void RenderViews(float p_deltaTime);

		void RenderEditorUI(float p_deltaTime);

		void PostUpdate();

	private:
		uint64_t m_elapsedFrames = 0;
		UI::UI_Canvas			m_canvas;
		Editor_Context& m_context;
		Editor_EditorRenderer	m_editorRenderer;
		Editor_PanelsManager	m_panelsManager;
		Editor_EditorActions	m_editorActions;
	};
}

