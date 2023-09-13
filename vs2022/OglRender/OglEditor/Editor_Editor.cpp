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
	mContext(p_context),
	m_editorRenderer(p_context),
	m_panelsManager(mCanvas),
	m_editorActions(mContext, m_editorRenderer, m_panelsManager)
{
	SetupUI();

	mContext.sceneManager.LoadEmptyLightedScene();
}

Editor::Editor_Editor::~Editor_Editor()
{
	mContext.sceneManager.UnloadCurrentScene();
}

void Editor::Editor_Editor::SetupUI()
{
	UI::UI_PanelWindowSettings settings;
	settings.closable = true;
	settings.collapsable = true;
	settings.dockable = true;

	m_panelsManager.CreatePanel<Editor_MenuBar>("Menu Bar");
	m_panelsManager.CreatePanel<Editor_AssetBrowser>("Asset Browser", true, settings, mContext.mEngineAssetsPath, mContext.mProjectAssetsPath, mContext.mProjectScriptsPath);
	m_panelsManager.CreatePanel<Editor_HardwareInfo>("Hardware Info", false, settings, 0.2f, 50);
	m_panelsManager.CreatePanel<Editor_Profiler>("Profiler", true, settings, 0.25f);
	m_panelsManager.CreatePanel<Editor_Console>("Console", true, settings);
	m_panelsManager.CreatePanel<Editor_Hierarchy>("Hierarchy", true, settings);
	m_panelsManager.CreatePanel<Editor_Inspector>("Inspector", true, settings);
	m_panelsManager.CreatePanel<Editor_SceneView>("Scene View", true, settings);
	m_panelsManager.CreatePanel<Editor_GameView>("Game View", true, settings);
	m_panelsManager.CreatePanel<Editor_AssetView>("Asset View", false, settings);
	m_panelsManager.CreatePanel<Editor_Toolbar>("Toolbar", true, settings);
	m_panelsManager.CreatePanel<Editor_MaterialEditor>("Material Editor", false, settings);
	m_panelsManager.CreatePanel<Editor_ProjectSettings>("Project Settings", false, settings);
	m_panelsManager.CreatePanel<Editor_AssetProperties>("Asset Properties", false, settings);

	mCanvas.MakeDockspace(true);
	mContext.uiManager->SetCanvas(mCanvas);
}

void Editor::Editor_Editor::PreUpdate()
{
	//PROFILER_SPY("Editor Pre-Update");

	mContext.device->PollEvents();
	mContext.renderer->SetClearColor(0.f, 0.f, 0.f);
	mContext.renderer->Clear();
}

void Editor::Editor_Editor::Update(float pDeltaTime)
{
	HandleGlobalShortcuts();
	UpdateCurrentEditorMode(pDeltaTime);
	PrepareRendering(pDeltaTime);
	UpdateEditorPanels(pDeltaTime);
	RenderViews(pDeltaTime);
	RenderEditorUI(pDeltaTime);
	m_editorActions.ExecuteDelayedActions();
}

void Editor::Editor_Editor::HandleGlobalShortcuts()
{
	if (mContext.inputManager->IsKeyPressed(Window::EKey::KEY_DELETE) && EDITOR_EXEC(IsAnyActorSelected()) && (EDITOR_PANEL(Editor_SceneView, "Scene View").IsFocused() || EDITOR_PANEL(Editor_Hierarchy, "Hierarchy").IsFocused()))
	{
		EDITOR_EXEC(DestroyActor(EDITOR_EXEC(GetSelectedActor())));
	}
}

void Editor::Editor_Editor::UpdateCurrentEditorMode(float pDeltaTime)
{
	if (auto editorMode = m_editorActions.GetCurrentEditorMode(); editorMode == Editor_EditorActions::EEditorMode::PLAY || editorMode == Editor_EditorActions::EEditorMode::FRAME_BY_FRAME)
	{
		UpdatePlayMode(pDeltaTime);
	}
	else
	{
		UpdateEditMode(pDeltaTime);
	}

	{
		//PROFILER_SPY("Scene garbage collection");
		mContext.sceneManager.GetCurrentScene()->CollectGarbages();
		mContext.sceneManager.Update();
	}
}

void Editor::Editor_Editor::UpdatePlayMode(float pDeltaTime)
{
	auto currentScene = mContext.sceneManager.GetCurrentScene();
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

	ImGui::GetIO().DisableMouseUpdate = mContext.window->GetCursorMode() == Window::ECursorMode::DISABLED;

	if (m_editorActions.GetCurrentEditorMode() == Editor_EditorActions::EEditorMode::FRAME_BY_FRAME)
	{
		m_editorActions.PauseGame();
	}

	if (mContext.inputManager->IsKeyPressed(Window::EKey::KEY_ESCAPE))
	{
		m_editorActions.StopPlaying();
	}
}

void Editor::Editor_Editor::UpdateEditMode(float pDeltaTime)
{
	if (mContext.inputManager->IsKeyPressed(Window::EKey::KEY_F5))
	{
		m_editorActions.StartPlaying();
	}
}

void Editor::Editor_Editor::UpdateEditorPanels(float pDeltaTime)
{
	auto& menuBar = m_panelsManager.GetPanelAs<Editor_MenuBar>("Menu Bar");
	auto& profiler = m_panelsManager.GetPanelAs<Editor_Profiler>("Profiler");
	auto& hardwareInfo = m_panelsManager.GetPanelAs<Editor_HardwareInfo>("Hardware Info");
	auto& sceneView = m_panelsManager.GetPanelAs<Editor_SceneView>("Scene View");

	menuBar.HandleShortcuts(pDeltaTime);

	if (mElapsedFrames == 1)
	{
		sceneView.Focus();
	}

	if (profiler.IsOpened())
	{
		//PROFILER_SPY("Profiler Update");
		//profiler.Update(p_deltaTime);
	}

	if (hardwareInfo.IsOpened())
	{
		//PROFILER_SPY("Hardware Info Update");
		hardwareInfo.Update(pDeltaTime);
	}
}

void Editor::Editor_Editor::PrepareRendering(float pDeltaTime)
{
	//PROFILER_SPY("Engine UBO Update");
	mContext.engineUBO->SetSubData(mContext.device->GetElapsedTime(), 3 * sizeof(glm::mat4) + sizeof(glm::vec3));
}

void Editor::Editor_Editor::RenderViews(float pDeltaTime)
{
	auto& assetView = m_panelsManager.GetPanelAs<Editor_AssetView>("Asset View");
	auto& sceneView = m_panelsManager.GetPanelAs<Editor_SceneView>("Scene View");
	auto& gameView = m_panelsManager.GetPanelAs<Editor_GameView>("Game View");

	{
		//PROFILER_SPY("Editor Views Update");

		assetView.Update(pDeltaTime);
		gameView.Update(pDeltaTime);
		sceneView.Update(pDeltaTime);
	}

	if (assetView.IsOpened())
	{
		//PROFILER_SPY("Asset View Rendering");

		mContext.simulatedLightSSBO->Bind(0);
		assetView.Render();
		mContext.simulatedLightSSBO->Unbind();
	}

	mContext.lightSSBO->Bind(0);

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

	mContext.lightSSBO->Unbind();
}

void Editor::Editor_Editor::RenderEditorUI(float pDeltaTime)
{
	//PROFILER_SPY("Editor UI Rendering");

	m_editorActions.GetRenderer().RenderUI();
}

void Editor::Editor_Editor::PostUpdate()
{
	//PROFILER_SPY("Editor Post-Update");

	mContext.window->SwapBuffers();
	mContext.inputManager->ClearEvents();
	++mElapsedFrames;
}
