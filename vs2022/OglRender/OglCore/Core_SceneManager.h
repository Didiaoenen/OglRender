#pragma once

#include "Core_Scene.h"

namespace Core
{
	class Core_SceneManager
	{
	public:
		Core_SceneManager(const std::string& pSceneRootFolder = "");

		~Core_SceneManager();

		void Update();

		void LoadAndPlayDelayed(const std::string& pPath, bool pAbsolute = false);

		void LoadEmptyScene();

		void LoadEmptyLightedScene();

		bool LoadScene(const std::string& pPath, bool pAbsolute = false);

		bool LoadSceneFromMemory(tinyxml2::XMLDocument& pDoc);

		void UnloadCurrentScene();

		bool HasCurrentScene() const;

		Core_Scene* GetCurrentScene();

		std::string GetCurrentSceneSourcePath() const;

		bool IsCurrentSceneLoadedFromDisk() const;

		void StoreCurrentSceneSourcePath(const std::string& pPath);

		void ForgetCurrentSceneSourcePath();

	public:
		Tools::Tools_Event<> mSceneLoadEvent;
		Tools::Tools_Event<> mSceneUnloadEvent;
		Tools::Tools_Event<const std::string&> mCurrentSceneSourcePathChangedEvent;

	private:
		const std::string mSceneRootFolder{ "" };
		Core_Scene* mCurrentScene{ nullptr };

		bool mCurrentSceneLoadedFromPath{ false };
		std::string mCurrentSceneSourcePath{ "" };

		std::function<void()> mDelayedLoadCall;
	};
}

