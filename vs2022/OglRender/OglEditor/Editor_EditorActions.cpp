#include <fstream>
#include <iostream>
#include <filesystem>

#include <OglCore/Core_CModelRenderer.h>
#include <OglCore/Core_CMaterialRenderer.h>

#include <OglWindow/Window_OpenFileDialog.h>
#include <OglWindow/Window_SaveFileDialog.h>
#include <OglWindow/Window_MessageBox.h>

#include <OglTools/Tools_String.h>
#include <OglTools/Tools_PathParser.h>
#include <OglTools/Tools_SystemCalls.h>

#include "Editor_GameView.h"
#include "Editor_SceneView.h"
#include "Editor_AssetView.h"
#include "Editor_Inspector.h"
#include "Editor_EditorActions.h"
#include "Editor_ProjectSettings.h"
#include "Editor_MaterialEditor.h"

#include "Editor_EditorActions.h"

Editor::Editor_EditorActions::Editor_EditorActions(Editor_Context& p_context, Editor_EditorRenderer& p_editorRenderer, Editor_PanelsManager& p_panelsManager) :
	m_context(p_context),
	m_renderer(p_editorRenderer),
	m_panelsManager(p_panelsManager)
{
}

Editor::Editor_Context& Editor::Editor_EditorActions::GetContext()
{
	return m_context;
}

Editor::Editor_EditorRenderer& Editor::Editor_EditorActions::GetRenderer()
{
	return m_renderer;
}

Editor::Editor_PanelsManager& Editor::Editor_EditorActions::GetPanelsManager()
{
	return m_panelsManager;
}

void Editor::Editor_EditorActions::SetActorSpawnAtOrigin(bool pValue)
{
}

void Editor::Editor_EditorActions::SetActorSpawnMode(EActorSpawnMode pValue)
{
}

void Editor::Editor_EditorActions::ResetLayout()
{
}

void Editor::Editor_EditorActions::SetSceneViewCameraSpeed(int p_speed)
{
}

int Editor::Editor_EditorActions::GetSceneViewCameraSpeed()
{
	return 0;
}

void Editor::Editor_EditorActions::SetAssetViewCameraSpeed(int p_speed)
{
}

int Editor::Editor_EditorActions::GetAssetViewCameraSpeed()
{
	return 0;
}

void Editor::Editor_EditorActions::ResetSceneViewCameraPosition()
{
}

void Editor::Editor_EditorActions::ResetAssetViewCameraPosition()
{
}

Editor::Editor_EditorActions::EEditorMode Editor::Editor_EditorActions::GetCurrentEditorMode() const
{
	return EEditorMode();
}

void Editor::Editor_EditorActions::SetEditorMode(EEditorMode p_newEditorMode)
{
}

void Editor::Editor_EditorActions::StartPlaying()
{
}

void Editor::Editor_EditorActions::PauseGame()
{
}

void Editor::Editor_EditorActions::StopPlaying()
{
}

void Editor::Editor_EditorActions::NextFrame()
{
}

glm::vec3 Editor::Editor_EditorActions::CalculateActorSpawnPoint(float p_distanceToCamera)
{
	return glm::vec3();
}

Core::Core_Actor& Editor::Editor_EditorActions::CreateEmptyActor(bool p_focusOnCreation, Core::Core_Actor* p_parent, const std::string& p_name)
{
	const auto currentScene = m_context.sceneManager.GetCurrentScene();
	auto& instance = p_name.empty() ? currentScene->CreateActor() : currentScene->CreateActor(p_name);

	if (p_parent)
		instance.SetParent(*p_parent);

	if (m_actorSpawnMode == EActorSpawnMode::FRONT)
		instance.transform.SetLocalPosition(CalculateActorSpawnPoint(10.0f));

	if (p_focusOnCreation)
		SelectActor(instance);

	//OVLOG_INFO("Actor created");

	return instance;
}

Core::Core_Actor& Editor::Editor_EditorActions::CreateActorWithModel(const std::string& pPath, bool p_focusOnCreation, Core::Core_Actor* p_parent, const std::string& p_name)
{
	auto& instance = CreateEmptyActor(false, p_parent, p_name);

	auto& modelRenderer = instance.AddComponent<Core::Core_CModelRenderer>();

	const auto model = m_context.modelManager[pPath];
	if (model)
		modelRenderer.SetModel(model);

	auto& materialRenderer = instance.AddComponent<Core::Core_CMaterialRenderer>();
	const auto material = m_context.materialManager[":Materials\\Default.ovmat"];
	if (material)
		materialRenderer.FillWithMaterial(*material);

	if (p_focusOnCreation)
		SelectActor(instance);

	return instance;
}

bool Editor::Editor_EditorActions::DestroyActor(Core::Core_Actor& p_actor)
{
	return false;
}

void Editor::Editor_EditorActions::DuplicateActor(Core::Core_Actor& p_toDuplicate, Core::Core_Actor* p_forcedParent, bool p_focus)
{
}

void Editor::Editor_EditorActions::SelectActor(Core::Core_Actor& p_target)
{
}

void Editor::Editor_EditorActions::UnselectActor()
{
}

bool Editor::Editor_EditorActions::IsAnyActorSelected() const
{
	return false;
}

Core::Core_Actor& Editor::Editor_EditorActions::GetSelectedActor() const
{
	return *EDITOR_PANEL(Editor_Inspector, "Inspector").GetTargetActor();
}

void Editor::Editor_EditorActions::MoveToTarget(Core::Core_Actor& p_target)
{
}

void Editor::Editor_EditorActions::CompileShaders()
{
}

void Editor::Editor_EditorActions::SaveMaterials()
{
}

bool Editor::Editor_EditorActions::ImportAsset(const std::string& p_initialDestinationDirectory)
{
	return false;
}

bool Editor::Editor_EditorActions::ImportAssetAtLocation(const std::string& p_destination)
{
	return false;
}

std::string Editor::Editor_EditorActions::GetRealPath(const std::string& pPath)
{
	return std::string();
}

std::string Editor::Editor_EditorActions::GetResourcePath(const std::string& pPath, bool p_isFromEngine)
{
	return std::string();
}

std::string Editor::Editor_EditorActions::GetScriptPath(const std::string& pPath)
{
	return std::string();
}

void Editor::Editor_EditorActions::PropagateFolderRename(std::string p_previousName, const std::string p_newName)
{
}

void Editor::Editor_EditorActions::PropagateFolderDestruction(std::string p_folderPath)
{
}

void Editor::Editor_EditorActions::PropagateScriptRename(std::string p_previousName, std::string p_newName)
{
}

void Editor::Editor_EditorActions::PropagateFileRename(std::string p_previousName, std::string p_newName)
{
}

void Editor::Editor_EditorActions::PropagateFileRenameThroughSavedFilesOfType(const std::string& p_previousName, const std::string& p_newName, Tools::Tools_PathParser::EFileType pFileType)
{
}

void Editor::Editor_EditorActions::LoadEmptyScene()
{
}

void Editor::Editor_EditorActions::SaveCurrentSceneTo(const std::string& pPath)
{
}

void Editor::Editor_EditorActions::LoadSceneFromDisk(const std::string& pPath, bool p_absolute)
{
}

bool Editor::Editor_EditorActions::IsCurrentSceneLoadedFromDisk() const
{
	return false;
}

void Editor::Editor_EditorActions::SaveSceneChanges()
{
}

void Editor::Editor_EditorActions::SaveAs()
{
}

void Editor::Editor_EditorActions::RefreshScripts()
{
}

std::optional<std::string> Editor::Editor_EditorActions::SelectBuildFolder()
{
	return std::optional<std::string>();
}

void Editor::Editor_EditorActions::Build(bool p_autoRun, bool p_tempFolder)
{
}

void Editor::Editor_EditorActions::BuildAtLocation(const std::string& p_configuration, const std::string p_buildPath, bool p_autoRun)
{
}

void Editor::Editor_EditorActions::DelayAction(std::function<void()> p_action, uint32_t p_frames)
{
}

void Editor::Editor_EditorActions::ExecuteDelayedActions()
{
}
