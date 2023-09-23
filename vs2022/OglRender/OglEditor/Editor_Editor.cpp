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

#include <OglUI/imgui.h>
#include <OglUI/imgui_internal.h>

#include <OglUI/UI_PanelSideBar.h>

#include "Editor_Editor.h"

Editor::Editor_Editor::Editor_Editor(Editor_Context& pContext) :
	mContext(pContext),
	mEditorRenderer(pContext),
	mPanelsManager(mCanvas),
	mEditorActions(mContext, mEditorRenderer, mPanelsManager)
{
	SetupUI();

	mContext.mSceneManager.LoadEmptyLightedScene();
}

Editor::Editor_Editor::~Editor_Editor()
{
	mContext.mSceneManager.UnloadCurrentScene();
}

void Editor::Editor_Editor::SetupUI()
{
	UI::UI_PanelWindowSettings settings;
	settings.closable = true;
	settings.collapsable = false;
	settings.dockable = true;

	mPanelsManager.CreatePanel<Editor_MenuBar>("Menu Bar");
	mPanelsManager.CreatePanel<Editor_Toolbar>("Second Bar");
	mPanelsManager.CreatePanel<Editor_AssetBrowser>("Assets", true, settings, mContext.mEngineAssetsPath, mContext.mProjectAssetsPath, mContext.mProjectScriptsPath);
	mPanelsManager.CreatePanel<Editor_HardwareInfo>("Hardware", false, settings, 0.2f, 50);
	mPanelsManager.CreatePanel<Editor_Profiler>("Profiler", true, settings, 0.25f);
	mPanelsManager.CreatePanel<Editor_Console>("Console", true, settings);
	mPanelsManager.CreatePanel<Editor_Hierarchy>("Hierarchy", true, settings);
	mPanelsManager.CreatePanel<Editor_Inspector>("Inspector", true, settings);
	mPanelsManager.CreatePanel<Editor_SceneView>("Scene", true, settings);
	mPanelsManager.CreatePanel<Editor_GameView>("Game", true, settings);
	mPanelsManager.CreatePanel<Editor_AssetView>("Asset", false, settings);
	mPanelsManager.CreatePanel<Editor_MaterialEditor>("Material Editor", false, settings);
	mPanelsManager.CreatePanel<Editor_ProjectSettings>("Project Settings", false, settings);
	mPanelsManager.CreatePanel<Editor_AssetProperties>("Asset Properties", false, settings);

	mCanvas.MakeDockspace(true);
	mContext.mUIManager->SetCanvas(mCanvas);
}

void Editor::Editor_Editor::PreUpdate()
{
	//PROFILER_SPY("Editor Pre-Update");

	mContext.mDevice->PollEvents();
	mContext.mRenderer->SetClearColor(0.f, 0.f, 0.f);
	mContext.mRenderer->Clear();
}

void Editor::Editor_Editor::Update(float pDeltaTime)
{
	HandleGlobalShortcuts();
	UpdateCurrentEditorMode(pDeltaTime);
	PrepareRendering(pDeltaTime);
	UpdateEditorPanels(pDeltaTime);
	RenderViews(pDeltaTime);
	RenderEditorUI(pDeltaTime);
	mEditorActions.ExecuteDelayedActions();
}

void Editor::Editor_Editor::HandleGlobalShortcuts()
{
	if (mContext.mInputManager->IsKeyPressed(Window::EKey::KEY_DELETE) && EDITOR_EXEC(IsAnyActorSelected()) && 
		(EDITOR_PANEL(Editor_SceneView, "Scene").IsFocused() || EDITOR_PANEL(Editor_Hierarchy, "Hierarchy").IsFocused()))
	{
		EDITOR_EXEC(DestroyActor(EDITOR_EXEC(GetSelectedActor())));
	}
}

void Editor::Editor_Editor::UpdateCurrentEditorMode(float pDeltaTime)
{
	auto editorMode = mEditorActions.GetCurrentEditorMode();
	if (editorMode == Editor_EditorActions::EEditorMode::PLAY || editorMode == Editor_EditorActions::EEditorMode::FRAME_BY_FRAME)
	{
		UpdatePlayMode(pDeltaTime);
	}
	else
	{
		UpdateEditMode(pDeltaTime);
	}

	{
		//PROFILER_SPY("Scene garbage collection");
		mContext.mSceneManager.GetCurrentScene()->CollectGarbages();
		mContext.mSceneManager.Update();
	}
}

void Editor::Editor_Editor::UpdatePlayMode(float pDeltaTime)
{
	auto currentScene = mContext.mSceneManager.GetCurrentScene();
	bool simulationApplied = false;

	{
		//PROFILER_SPY("Physics Update");
		//simulationApplied = mContext.physicsEngine->Update(p_deltaTime);
	}

	if (simulationApplied)
	{
		//PROFILER_SPY("FixedUpdate");
		currentScene->FixedUpdate(pDeltaTime);
	}

	{
		//PROFILER_SPY("Update");
		currentScene->Update(pDeltaTime);
	}

	{
		//PROFILER_SPY("LateUpdate");
		currentScene->LateUpdate(pDeltaTime);
	}

	{
		//PROFILER_SPY("Audio Update");
		//m_context.audioEngine->Update();
	}

	ImGui::GetIO().DisableMouseUpdate = mContext.mWindow->GetCursorMode() == Window::ECursorMode::DISABLED;

	if (mEditorActions.GetCurrentEditorMode() == Editor_EditorActions::EEditorMode::FRAME_BY_FRAME)
	{
		mEditorActions.PauseGame();
	}

	if (mContext.mInputManager->IsKeyPressed(Window::EKey::KEY_ESCAPE))
	{
		mEditorActions.StopPlaying();
	}
}

void Editor::Editor_Editor::UpdateEditMode(float pDeltaTime)
{
	if (mContext.mInputManager->IsKeyPressed(Window::EKey::KEY_F5))
	{
		mEditorActions.StartPlaying();
	}
}

void Editor::Editor_Editor::UpdateEditorPanels(float pDeltaTime)
{
	auto& menuBar = mPanelsManager.GetPanelAs<Editor_MenuBar>("Menu Bar");
	auto& profiler = mPanelsManager.GetPanelAs<Editor_Profiler>("Profiler");
	auto& hardwareInfo = mPanelsManager.GetPanelAs<Editor_HardwareInfo>("Hardware");
	auto& sceneView = mPanelsManager.GetPanelAs<Editor_SceneView>("Scene");

	menuBar.HandleShortcuts(pDeltaTime);

	if (mElapsedFrames == 1)
	{
		sceneView.Focus();
	}

	//if (profiler.IsOpened())
	//{
	//	//PROFILER_SPY("Profiler Update");
	//	//profiler.Update(p_deltaTime);
	//}

	//if (hardwareInfo.IsOpened())
	//{
	//	//PROFILER_SPY("Hardware Info Update");
	//	hardwareInfo.Update(pDeltaTime);
	//}
}

void Editor::Editor_Editor::PrepareRendering(float pDeltaTime)
{
	//PROFILER_SPY("Engine UBO Update");
	mContext.mEngineUBO->SetSubData(mContext.mDevice->GetElapsedTime(), 3 * sizeof(glm::mat4) + sizeof(glm::vec3));
}

void Editor::Editor_Editor::RenderViews(float pDeltaTime)
{
	auto& assetView = mPanelsManager.GetPanelAs<Editor_AssetView>("Asset");
	auto& sceneView = mPanelsManager.GetPanelAs<Editor_SceneView>("Scene");
	auto& gameView = mPanelsManager.GetPanelAs<Editor_GameView>("Game");

	{
		//PROFILER_SPY("Editor Views Update");

		assetView.Update(pDeltaTime);
		gameView.Update(pDeltaTime);
		sceneView.Update(pDeltaTime);
	}

	if (assetView.IsOpened())
	{
		//PROFILER_SPY("Asset View Rendering");

		mContext.mSimulatedLightSSBO->Bind(0);
		assetView.Render();
		mContext.mSimulatedLightSSBO->Unbind();
	}

	mContext.mLightSSBO->Bind(0);

	if (gameView.IsOpened())
	{
		//PROFILER_SPY("Game View Rendering");

		gameView.Render();
	}

	if (sceneView.IsOpened())
	{
		//PROFILER_SPY("Scene View Rendering");

		sceneView.Render();
	}

	mContext.mLightSSBO->Unbind();
}

void Editor::Editor_Editor::RenderEditorUI(float pDeltaTime)
{
	//PROFILER_SPY("Editor UI Rendering");

	mEditorActions.GetRenderer().RenderUI();
}

void Editor::Editor_Editor::PostUpdate()
{
	//PROFILER_SPY("Editor Post-Update");

	mContext.mWindow->SwapBuffers();
	mContext.mInputManager->ClearEvents();
	++mElapsedFrames;
}
