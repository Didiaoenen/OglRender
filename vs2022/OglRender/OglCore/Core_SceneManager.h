#pragma once

#include "Core_Scene.h"

namespace Core
{
	class Core_SceneManager
	{
	public:
		Core_SceneManager(const std::string& p_sceneRootFolder = "");

		~Core_SceneManager();

		void Update();

		void LoadAndPlayDelayed(const std::string& pPath, bool p_absolute = false);

		void LoadEmptyScene();

		void LoadEmptyLightedScene();

		bool LoadScene(const std::string& pPath, bool p_absolute = false);

		bool LoadSceneFromMemory(tinyxml2::XMLDocument& pDoc);

		void UnloadCurrentScene();

		bool HasCurrentScene() const;

		Core_Scene* GetCurrentScene();

		std::string GetCurrentSceneSourcePath() const;

		bool IsCurrentSceneLoadedFromDisk() const;

		void StoreCurrentSceneSourcePath(const std::string& pPath);

		void ForgetCurrentSceneSourcePath();

	public:
		Tools::Tools_Event<> SceneLoadEvent;
		Tools::Tools_Event<> SceneUnloadEvent;
		Tools::Tools_Event<const std::string&> CurrentSceneSourcePathChangedEvent;

	private:
		const std::string m_sceneRootFolder;
		Core_Scene* m_currentScene{ nullptr };

		bool m_currentSceneLoadedFromPath{ false };
		std::string m_currentSceneSourcePath{ "" };

		std::function<void()> m_delayedLoadCall;
	};
}

