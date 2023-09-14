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
		Editor_Editor(Editor_Context& pContext);

		~Editor_Editor();

		void SetupUI();

		void PreUpdate();

		void Update(float pDeltaTime);

		void HandleGlobalShortcuts();

		void UpdateCurrentEditorMode(float pDeltaTime);

		void UpdatePlayMode(float pDeltaTime);

		void UpdateEditMode(float pDeltaTime);

		void UpdateEditorPanels(float pDeltaTime);

		void PrepareRendering(float pDeltaTime);

		void RenderViews(float pDeltaTime);

		void RenderEditorUI(float pDeltaTime);

		void PostUpdate();

	private:
		uint64_t mElapsedFrames = 0;
		UI::UI_Canvas			mCanvas;
		Editor_Context&			mContext;
		Editor_EditorRenderer	mEditorRenderer;
		Editor_PanelsManager	mPanelsManager;
		Editor_EditorActions	mEditorActions;
	};
}

