#include <OglTools/tinyxml2.h>
#include <OglWindow/Window_MessageBox.h>

#include "Core_CAmbientSphereLight.h"
#include "Core_CDirectionalLight.h"
#include "Core_CCamera.h"

#include "Core_SceneManager.h"

Core::Core_SceneManager::Core_SceneManager(const std::string& pSceneRootFolder)
{
	LoadEmptyScene();
}

Core::Core_SceneManager::~Core_SceneManager()
{
	UnloadCurrentScene();
}

void Core::Core_SceneManager::Update()
{
	if (mDelayedLoadCall)
	{
		mDelayedLoadCall();
		mDelayedLoadCall = nullptr;
	}
}

void Core::Core_SceneManager::LoadAndPlayDelayed(const std::string& pPath, bool pAbsolute)
{
	mDelayedLoadCall = [this, pPath, pAbsolute]
		{
			std::string previousSourcePath = GetCurrentSceneSourcePath();
			LoadScene(pPath, pAbsolute);
			StoreCurrentSceneSourcePath(previousSourcePath);
			GetCurrentScene()->Play();
		};
}

void Core::Core_SceneManager::LoadEmptyScene()
{
	UnloadCurrentScene();

	mCurrentScene = new Core_Scene();

	mSceneLoadEvent.Invoke();
}

void Core::Core_SceneManager::LoadEmptyLightedScene()
{
	UnloadCurrentScene();

	mCurrentScene = new Core_Scene();

	mSceneLoadEvent.Invoke();

	auto& directionalLight = mCurrentScene->CreateActor("Directional Light");
	directionalLight.AddComponent<Core_CDirectionalLight>().SetIntensity(0.75f);
	directionalLight.transform.SetLocalPosition({ 0.0f, 10.0f, 0.0f });
	directionalLight.transform.SetLocalRotation(glm::quat({ 120.0f, -40.0f, 0.0f }));

	auto& ambientLight = mCurrentScene->CreateActor("Ambient Light");
	ambientLight.AddComponent<Core_CAmbientSphereLight>().SetRadius(10000.0f);

	auto& camera = mCurrentScene->CreateActor("Main Camera");
	camera.AddComponent<Core_CCamera>();
	camera.transform.SetLocalPosition({ 0.0f, 3.0f, 8.0f });
	camera.transform.SetLocalRotation(glm::quat({ 20.0f, 180.0f, 0.0f }));
}

bool Core::Core_SceneManager::LoadScene(const std::string & pPath, bool pAbsolute)
{
	std::string completePath = (pAbsolute ? "" : mSceneRootFolder) + pPath;

	tinyxml2::XMLDocument doc;
	doc.LoadFile(completePath.c_str());

	if (LoadSceneFromMemory(doc))
	{
		StoreCurrentSceneSourcePath(completePath);
		return true;
	}

	return false;
}

bool Core::Core_SceneManager::LoadSceneFromMemory(tinyxml2::XMLDocument& pDoc)
{
	if (!pDoc.Error())
	{
		tinyxml2::XMLNode* root = pDoc.FirstChild();
		if (root)
		{
			tinyxml2::XMLNode* sceneNode = root->FirstChildElement("scene");
			if (sceneNode)
			{
				LoadEmptyScene();
				mCurrentScene->OnDeserialize(pDoc, sceneNode);
				return true;
			}
		}
	}

	Window::Window_MessageBox message("Scene loading failed", "The scene you are trying to load was not found or corrupted", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK, true);
	return false;
}

void Core::Core_SceneManager::UnloadCurrentScene()
{
	if (mCurrentScene)
	{
		delete mCurrentScene;
		mCurrentScene = nullptr;
		mSceneUnloadEvent.Invoke();
	}

	ForgetCurrentSceneSourcePath();
}

bool Core::Core_SceneManager::HasCurrentScene() const
{
	return mCurrentScene;
}

Core::Core_Scene* Core::Core_SceneManager::GetCurrentScene()
{
	return mCurrentScene;
}

std::string Core::Core_SceneManager::GetCurrentSceneSourcePath() const
{
	return mCurrentSceneSourcePath;
}

bool Core::Core_SceneManager::IsCurrentSceneLoadedFromDisk() const
{
	return mCurrentSceneLoadedFromPath;
}

void Core::Core_SceneManager::StoreCurrentSceneSourcePath(const std::string& pPath)
{
	mCurrentSceneSourcePath = pPath;
	mCurrentSceneLoadedFromPath = true;
	mCurrentSceneSourcePathChangedEvent.Invoke(mCurrentSceneSourcePath);
}

void Core::Core_SceneManager::ForgetCurrentSceneSourcePath()
{
	mCurrentSceneSourcePath = "";
	mCurrentSceneLoadedFromPath = false;
	mCurrentSceneSourcePathChangedEvent.Invoke(mCurrentSceneSourcePath);
}
