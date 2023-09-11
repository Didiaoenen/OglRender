#pragma once

#include <OglCore/Core_ServiceLocator.h>

#include <OglTools/Tools_IniFile.h>
#include <OglTools/Tools_PathParser.h>

#include "Editor_Context.h"
#include "Editor_PanelsManager.h"
#include "Editor_EditorRenderer.h"

#define EDITOR_EXEC(action)					Core::Core_ServiceLocator::Get<Editor::Editor_EditorActions>().action
#define EDITOR_BIND(method, ...)			std::bind(&Editor::Editor_EditorActions::method, &Core::Core_ServiceLocator::Get<Editor::Editor_EditorActions>(), ##__VA_ARGS__)
#define EDITOR_EVENT(target)				Core::Core_ServiceLocator::Get<Editor::Editor_EditorActions>().target
#define EDITOR_CONTEXT(instance)			Core::Core_ServiceLocator::Get<Editor::Editor_EditorActions>().GetContext().instance
#define EDITOR_RENDERER()					Core::Core_ServiceLocator::Get<Editor::Editor_EditorActions>().GetRenderer()
#define EDITOR_PANEL(type, id)				Core::Core_ServiceLocator::Get<Editor::Editor_EditorActions>().GetPanelsManager().GetPanelAs<type>(id)

namespace Editor
{
	class Editor_EditorActions
	{
	public:
		Editor_EditorActions(Editor_Context& p_context, Editor_EditorRenderer& p_editorRenderer, Editor_PanelsManager& p_panelsManager);

#pragma region TOOLS
		Editor_Context& GetContext();

		Editor_EditorRenderer& GetRenderer();

		Editor_PanelsManager& GetPanelsManager();
#pragma endregion

#pragma region SETTINGS
		enum class EActorSpawnMode { ORIGIN, FRONT };

		void SetActorSpawnAtOrigin(bool p_value);

		void SetActorSpawnMode(EActorSpawnMode p_value);

		void ResetLayout();

		void SetSceneViewCameraSpeed(int p_speed);

		int GetSceneViewCameraSpeed();

		void SetAssetViewCameraSpeed(int p_speed);

		int GetAssetViewCameraSpeed();

		void ResetSceneViewCameraPosition();

		void ResetAssetViewCameraPosition();
#pragma endregion

#pragma region GAME
		enum class EEditorMode { EDIT, PLAY, PAUSE, FRAME_BY_FRAME };

		EEditorMode GetCurrentEditorMode() const;

		void SetEditorMode(EEditorMode p_newEditorMode);

		void StartPlaying();

		void PauseGame();

		void StopPlaying();

		void NextFrame();
#pragma endregion

#pragma region ACTOR_CREATION_DESTRUCTION
		template<typename T> Core::Core_Actor& CreateMonoComponentActor(bool p_focusOnCreation = true, Core::Core_Actor* p_parent = nullptr);

		glm::vec3 CalculateActorSpawnPoint(float p_distanceToCamera);

		Core::Core_Actor& CreateEmptyActor(bool p_focusOnCreation = true, Core::Core_Actor* p_parent = nullptr, const std::string& p_name = "");

		Core::Core_Actor& CreateActorWithModel(const std::string& p_path, bool p_focusOnCreation = true, Core::Core_Actor* p_parent = nullptr, const std::string& p_name = "");

		bool DestroyActor(Core::Core_Actor& p_actor);

		void DuplicateActor(Core::Core_Actor& p_toDuplicate, Core::Core_Actor* p_forcedParent = nullptr, bool p_focus = true);
#pragma endregion

#pragma region ACTOR_MANIPULATION
		void SelectActor(Core::Core_Actor& p_target);

		void UnselectActor();

		bool IsAnyActorSelected() const;

		Core::Core_Actor& GetSelectedActor() const;

		void MoveToTarget(Core::Core_Actor& p_target);
#pragma endregion

#pragma region RESOURCE_MANAGEMENT
		void CompileShaders();

		void SaveMaterials();

		bool ImportAsset(const std::string& p_initialDestinationDirectory);

		bool ImportAssetAtLocation(const std::string& p_destination);

		std::string GetRealPath(const std::string& p_path);

		std::string GetResourcePath(const std::string& p_path, bool p_isFromEngine = false);

		std::string GetScriptPath(const std::string& p_path);

		void PropagateFolderRename(std::string p_previousName, const std::string p_newName);

		void PropagateFolderDestruction(std::string p_folderPath);

		void PropagateScriptRename(std::string p_previousName, std::string p_newName);

		void PropagateFileRename(std::string p_previousName, std::string p_newName);

		void PropagateFileRenameThroughSavedFilesOfType(const std::string& p_previousName, const std::string& p_newName, Tools::Tools_PathParser::EFileType p_fileType);
#pragma endregion

#pragma region SCENE
		void LoadEmptyScene();

		void SaveCurrentSceneTo(const std::string& p_path);

		void LoadSceneFromDisk(const std::string& p_path, bool p_absolute = false);

		bool IsCurrentSceneLoadedFromDisk() const;

		void SaveSceneChanges();

		void SaveAs();
#pragma endregion

#pragma region SCRIPTING
		void RefreshScripts();
#pragma endregion

#pragma region BUILDING
		std::optional<std::string> SelectBuildFolder();

		void Build(bool p_autoRun = false, bool p_tempFolder = false);

		void BuildAtLocation(const std::string& p_configuration, const std::string p_buildPath, bool p_autoRun = false);
#pragma endregion

#pragma region ACTION_SYSTEM
		void DelayAction(std::function<void()> p_action, uint32_t p_frames = 1);

		void ExecuteDelayedActions();
#pragma endregion

	public:
		Tools::Tools_Event<Core::Core_Actor&> ActorSelectedEvent;
		Tools::Tools_Event<Core::Core_Actor&> ActorUnselectedEvent;
		Tools::Tools_Event<EEditorMode> EditorModeChangedEvent;
		Tools::Tools_Event<> PlayEvent;

	private:
		Editor_Context& m_context;
		Editor_PanelsManager& m_panelsManager;
		Editor_EditorRenderer& m_renderer;

		EActorSpawnMode m_actorSpawnMode = EActorSpawnMode::ORIGIN;
		EEditorMode m_editorMode = EEditorMode::EDIT;

		std::vector<std::pair<uint32_t, std::function<void()>>> m_delayedActions;

		tinyxml2::XMLDocument m_sceneBackup;
	};
}

#include "Editor_EditorActions.inl"