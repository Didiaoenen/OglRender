#include <string>
#include <algorithm>

#include "Core_Scene.h"

Core::Core_Scene::Core_Scene()
{
}

Core::Core_Scene::~Core_Scene()
{
	std::for_each(m_actors.begin(), m_actors.end(), [](Core_Actor* element)
		{
			delete element;
		});

	m_actors.clear();
}

void Core::Core_Scene::Play()
{
	m_isPlaying = true;

	std::for_each(m_actors.begin(), m_actors.end(), [](Core_Actor* p_element) { p_element->SetSleeping(false); });

	std::for_each(m_actors.begin(), m_actors.end(), [](Core_Actor* p_element) { if (p_element->IsActive()) p_element->OnAwake(); });
	std::for_each(m_actors.begin(), m_actors.end(), [](Core_Actor* p_element) { if (p_element->IsActive()) p_element->OnEnable(); });
	std::for_each(m_actors.begin(), m_actors.end(), [](Core_Actor* p_element) { if (p_element->IsActive()) p_element->OnStart(); });
}

bool Core::Core_Scene::IsPlaying() const
{
	return m_isPlaying;
}

void Core::Core_Scene::Update(float pDeltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core_Actor::OnUpdate), std::placeholders::_1, pDeltaTime));
}

void Core::Core_Scene::FixedUpdate(float pDeltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core_Actor::OnFixedUpdate), std::placeholders::_1, pDeltaTime));
}

void Core::Core_Scene::LateUpdate(float pDeltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core_Actor::OnLateUpdate), std::placeholders::_1, pDeltaTime));
}

Core::Core_Actor& Core::Core_Scene::CreateActor()
{
	return CreateActor("New Actor");
}

Core::Core_Actor& Core::Core_Scene::CreateActor(const std::string& pName, const std::string& pTag)
{
	m_actors.push_back(new Core_Actor(m_availableID++, pName, pTag, m_isPlaying));
	Core_Actor& instance = *m_actors.back();
	instance.mComponentAddedEvent += std::bind(&Core_Scene::OnComponentAdded, this, std::placeholders::_1);
	instance.mComponentRemovedEvent += std::bind(&Core_Scene::OnComponentRemoved, this, std::placeholders::_1);
	if (m_isPlaying)
	{
		instance.SetSleeping(false);
		if (instance.IsActive())
		{
			instance.OnAwake();
			instance.OnEnable();
			instance.OnStart();
		}
	}
	return instance;
}

bool Core::Core_Scene::DestroyActor(Core::Core_Actor& pTarget)
{
	auto found = std::find_if(m_actors.begin(), m_actors.end(), [&pTarget](Core_Actor* element)
		{
			return element == &pTarget;
		});

	if (found != m_actors.end())
	{
		delete* found;
		m_actors.erase(found);
		return true;
	}
	else
	{
		return false;
	}
}

void Core::Core_Scene::CollectGarbages()
{
	m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(), [this](Core_Actor* element)
		{
			bool isGarbage = !element->IsAlive();
			if (isGarbage)
			{
				delete element;
			}
			return isGarbage;
		}), m_actors.end());
}

Core::Core_Actor* Core::Core_Scene::FindActorByName(const std::string& pName)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [pName](Core_Actor* element)
		{
			return element->GetName() == pName;
		});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

Core::Core_Actor* Core::Core_Scene::FindActorByTag(const std::string& pTag)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [pTag](Core_Actor* element)
		{
			return element->GetTag() == pTag;
		});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

Core::Core_Actor* Core::Core_Scene::FindActorByID(int64_t pId)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [pId](Core_Actor* element)
		{
			return element->GetID() == pId;
		});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

std::vector<std::reference_wrapper<Core::Core_Actor>> Core::Core_Scene::FindActorsByName(const std::string& pName)
{
	std::vector<std::reference_wrapper<Core_Actor>> actors;

	for (auto actor : m_actors)
	{
		if (actor->GetName() == pName)
			actors.push_back(std::ref(*actor));
	}

	return actors;
}

std::vector<std::reference_wrapper<Core::Core_Actor>> Core::Core_Scene::FindActorsByTag(const std::string& pTag)
{
	std::vector<std::reference_wrapper<Core_Actor>> actors;

	for (auto actor : m_actors)
	{
		if (actor->GetTag() == pTag)
			actors.push_back(std::ref(*actor));
	}

	return actors;
}

void Core::Core_Scene::OnComponentAdded(Core_AComponent& p_compononent)
{
	if (auto result = dynamic_cast<Core_CModelRenderer*>(&p_compononent))
		m_fastAccessComponents.modelRenderers.push_back(result);

	if (auto result = dynamic_cast<Core_CCamera*>(&p_compononent))
		m_fastAccessComponents.cameras.push_back(result);

	if (auto result = dynamic_cast<Core_CLight*>(&p_compononent))
		m_fastAccessComponents.lights.push_back(result);
}

void Core::Core_Scene::OnComponentRemoved(Core_AComponent& p_compononent)
{
	if (auto result = dynamic_cast<Core_CModelRenderer*>(&p_compononent))
		m_fastAccessComponents.modelRenderers.erase(std::remove(m_fastAccessComponents.modelRenderers.begin(), m_fastAccessComponents.modelRenderers.end(), result), m_fastAccessComponents.modelRenderers.end());

	if (auto result = dynamic_cast<Core_CCamera*>(&p_compononent))
		m_fastAccessComponents.cameras.erase(std::remove(m_fastAccessComponents.cameras.begin(), m_fastAccessComponents.cameras.end(), result), m_fastAccessComponents.cameras.end());

	if (auto result = dynamic_cast<Core_CLight*>(&p_compononent))
		m_fastAccessComponents.lights.erase(std::remove(m_fastAccessComponents.lights.begin(), m_fastAccessComponents.lights.end(), result), m_fastAccessComponents.lights.end());
}

std::vector<Core::Core_Actor*>& Core::Core_Scene::GetActors()
{
	return m_actors;
}

const Core::Core_Scene::FastAccessComponents& Core::Core_Scene::GetFastAccessComponents() const
{
	return m_fastAccessComponents;
}

void Core::Core_Scene::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pRoot)
{
	tinyxml2::XMLNode* sceneNode = pDoc.NewElement("scene");
	pRoot->InsertEndChild(sceneNode);

	tinyxml2::XMLNode* actorsNode = pDoc.NewElement("actors");
	sceneNode->InsertEndChild(actorsNode);

	for (auto& actor : m_actors)
	{
		actor->OnSerialize(pDoc, actorsNode);
	}
}

void Core::Core_Scene::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pRoot)
{
	tinyxml2::XMLNode* actorsRoot = pRoot->FirstChildElement("actors");

	if (actorsRoot)
	{
		tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");

		int64_t maxID = 1;

		while (currentActor)
		{
			auto& actor = CreateActor();
			actor.OnDeserialize(pDoc, currentActor);
			maxID = std::max(actor.GetID() + 1, maxID);
			currentActor = currentActor->NextSiblingElement("actor");
		}

		m_availableID = maxID;

		for (auto actor : m_actors)
		{
			if (actor->GetParentID() > 0)
			{
				if (auto found = FindActorByID(actor->GetParentID()); found)
					actor->SetParent(*found);
			}
		}
	}
}
