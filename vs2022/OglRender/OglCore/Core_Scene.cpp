#include <string>
#include <algorithm>

#include "Core_Scene.h"

Core::Core_Scene::Core_Scene()
{
}

Core::Core_Scene::~Core_Scene()
{
	std::for_each(m_actors.begin(), m_actors.end(), [](Core::Core_Actor* element)
		{
			delete element;
		});

	m_actors.clear();
}

void Core::Core_Scene::Play()
{
	m_isPlaying = true;

	std::for_each(m_actors.begin(), m_actors.end(), [](Core::Core_Actor* p_element) { p_element->SetSleeping(false); });

	std::for_each(m_actors.begin(), m_actors.end(), [](Core::Core_Actor* p_element) { if (p_element->IsActive()) p_element->OnAwake(); });
	std::for_each(m_actors.begin(), m_actors.end(), [](Core::Core_Actor* p_element) { if (p_element->IsActive()) p_element->OnEnable(); });
	std::for_each(m_actors.begin(), m_actors.end(), [](Core::Core_Actor* p_element) { if (p_element->IsActive()) p_element->OnStart(); });
}

bool Core::Core_Scene::IsPlaying() const
{
	return m_isPlaying;
}

void Core::Core_Scene::Update(float p_deltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core::Core_Actor::OnUpdate), std::placeholders::_1, p_deltaTime));
}

void Core::Core_Scene::FixedUpdate(float p_deltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core::Core_Actor::OnFixedUpdate), std::placeholders::_1, p_deltaTime));
}

void Core::Core_Scene::LateUpdate(float p_deltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core::Core_Actor::OnLateUpdate), std::placeholders::_1, p_deltaTime));
}

Core::Core_Actor& Core::Core_Scene::CreateActor()
{
	return CreateActor("New Actor");
}

Core::Core_Actor& Core::Core_Scene::CreateActor(const std::string& p_name, const std::string& p_tag)
{
	m_actors.push_back(new Core::Core_Actor(m_availableID++, p_name, p_tag, m_isPlaying));
	Core::Core_Actor& instance = *m_actors.back();
	instance.ComponentAddedEvent += std::bind(&Core_Scene::OnComponentAdded, this, std::placeholders::_1);
	instance.ComponentRemovedEvent += std::bind(&Core_Scene::OnComponentRemoved, this, std::placeholders::_1);
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

bool Core::Core_Scene::DestroyActor(Core::Core_Actor& p_target)
{
	auto found = std::find_if(m_actors.begin(), m_actors.end(), [&p_target](Core::Core_Actor* element)
		{
			return element == &p_target;
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
	m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(), [this](Core::Core_Actor* element)
		{
			bool isGarbage = !element->IsAlive();
			if (isGarbage)
			{
				delete element;
			}
			return isGarbage;
		}), m_actors.end());
}

Core::Core_Actor* Core::Core_Scene::FindActorByName(const std::string& p_name)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [p_name](Core_Actor* element)
		{
			return element->GetName() == p_name;
		});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

Core::Core_Actor* Core::Core_Scene::FindActorByTag(const std::string& p_tag)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [p_tag](Core_Actor* element)
		{
			return element->GetTag() == p_tag;
		});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

Core::Core_Actor* Core::Core_Scene::FindActorByID(int64_t p_id)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [p_id](Core_Actor* element)
		{
			return element->GetID() == p_id;
		});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

std::vector<std::reference_wrapper<Core::Core_Actor>> Core::Core_Scene::FindActorsByName(const std::string& p_name)
{
	std::vector<std::reference_wrapper<Core_Actor>> actors;

	for (auto actor : m_actors)
	{
		if (actor->GetName() == p_name)
			actors.push_back(std::ref(*actor));
	}

	return actors;
}

std::vector<std::reference_wrapper<Core::Core_Actor>> Core::Core_Scene::FindActorsByTag(const std::string& p_tag)
{
	std::vector<std::reference_wrapper<Core_Actor>> actors;

	for (auto actor : m_actors)
	{
		if (actor->GetTag() == p_tag)
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

void Core::Core_Scene::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_root)
{
	tinyxml2::XMLNode* sceneNode = p_doc.NewElement("scene");
	p_root->InsertEndChild(sceneNode);

	tinyxml2::XMLNode* actorsNode = p_doc.NewElement("actors");
	sceneNode->InsertEndChild(actorsNode);

	for (auto& actor : m_actors)
	{
		actor->OnSerialize(p_doc, actorsNode);
	}
}

void Core::Core_Scene::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_root)
{
	tinyxml2::XMLNode* actorsRoot = p_root->FirstChildElement("actors");

	if (actorsRoot)
	{
		tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");

		int64_t maxID = 1;

		while (currentActor)
		{
			auto& actor = CreateActor();
			actor.OnDeserialize(p_doc, currentActor);
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
