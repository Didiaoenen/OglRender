#include <OglTools/tinyxml2.h>
#include <OglWindow/Window_MessageBox.h>

#include "Core_CAmbientSphereLight.h"
#include "Core_CDirectionalLight.h"
#include "Core_CCamera.h"

#include "Core_SceneManager.h"

Core::Core_SceneManager::Core_SceneManager(const std::string& p_sceneRootFolder)
{
	LoadEmptyScene();
}

Core::Core_SceneManager::~Core_SceneManager()
{
	UnloadCurrentScene();
}

void Core::Core_SceneManager::Update()
{
	if (m_delayedLoadCall)
	{
		m_delayedLoadCall();
		m_delayedLoadCall = 0;
	}
}

void Core::Core_SceneManager::LoadAndPlayDelayed(const std::string& p_path, bool p_absolute)
{
	m_delayedLoadCall = [this, p_path, p_absolute]
		{
			std::string previousSourcePath = GetCurrentSceneSourcePath();
			LoadScene(p_path, p_absolute);
			StoreCurrentSceneSourcePath(previousSourcePath);
			GetCurrentScene()->Play();
		};
}

void Core::Core_SceneManager::LoadEmptyScene()
{
	UnloadCurrentScene();

	m_currentScene = new Core_Scene();

	SceneLoadEvent.Invoke();
}

void Core::Core_SceneManager::LoadEmptyLightedScene()
{
	UnloadCurrentScene();

	m_currentScene = new Core_Scene();

	SceneLoadEvent.Invoke();

	auto& directionalLight = m_currentScene->CreateActor("Directional Light");
	directionalLight.AddComponent<Core_CDirectionalLight>().SetIntensity(0.75f);
	directionalLight.transform.SetLocalPosition({ 0.0f, 10.0f, 0.0f });
	directionalLight.transform.SetLocalRotation(glm::quat({ 120.0f, -40.0f, 0.0f }));

	auto& ambientLight = m_currentScene->CreateActor("Ambient Light");
	ambientLight.AddComponent<Core_CAmbientSphereLight>().SetRadius(10000.0f);

	auto& camera = m_currentScene->CreateActor("Main Camera");
	camera.AddComponent<Core_CCamera>();
	camera.transform.SetLocalPosition({ 0.0f, 3.0f, 8.0f });
	camera.transform.SetLocalRotation(glm::quat({ 20.0f, 180.0f, 0.0f }));
}

bool Core::Core_SceneManager::LoadScene(const std::string & p_path, bool p_absolute)
{
	std::string completePath = (p_absolute ? "" : m_sceneRootFolder) + p_path;

	tinyxml2::XMLDocument doc;
	doc.LoadFile(completePath.c_str());

	if (LoadSceneFromMemory(doc))
	{
		StoreCurrentSceneSourcePath(completePath);
		return true;
	}

	return false;
}

bool Core::Core_SceneManager::LoadSceneFromMemory(tinyxml2::XMLDocument& p_doc)
{
	if (!p_doc.Error())
	{
		tinyxml2::XMLNode* root = p_doc.FirstChild();
		if (root)
		{
			tinyxml2::XMLNode* sceneNode = root->FirstChildElement("scene");
			if (sceneNode)
			{
				LoadEmptyScene();
				m_currentScene->OnDeserialize(p_doc, sceneNode);
				return true;
			}
		}
	}

	Window::Window_MessageBox message("Scene loading failed", "The scene you are trying to load was not found or corrupted", Window::Window_MessageBox::EMessageType::ERROR, Window::Window_MessageBox::EButtonLayout::OK, true);
	return false;
}

void Core::Core_SceneManager::UnloadCurrentScene()
{
	if (m_currentScene)
	{
		delete m_currentScene;
		m_currentScene = nullptr;
		SceneUnloadEvent.Invoke();
	}

	ForgetCurrentSceneSourcePath();
}

bool Core::Core_SceneManager::HasCurrentScene() const
{
	return m_currentScene;
}

Core::Core_Scene* Core::Core_SceneManager::GetCurrentScene()
{
	return m_currentScene;
}

std::string Core::Core_SceneManager::GetCurrentSceneSourcePath() const
{
	return m_currentSceneSourcePath;
}

bool Core::Core_SceneManager::IsCurrentSceneLoadedFromDisk() const
{
	return m_currentSceneLoadedFromPath;
}

void Core::Core_SceneManager::StoreCurrentSceneSourcePath(const std::string& p_path)
{
	m_currentSceneSourcePath = p_path;
	m_currentSceneLoadedFromPath = true;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}

void Core::Core_SceneManager::ForgetCurrentSceneSourcePath()
{
	m_currentSceneSourcePath = "";
	m_currentSceneLoadedFromPath = false;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}
