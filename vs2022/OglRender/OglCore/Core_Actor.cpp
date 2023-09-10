#include <algorithm>

#include "Core_CCamera.h"
#include "Core_CModelRenderer.h"
#include "Core_CMaterialRenderer.h"
#include "Core_CPointLight.h"
#include "Core_CSpotLight.h"
#include "Core_CDirectionalLight.h"
#include "Core_CAmbientBoxLight.h"
#include "Core_CAmbientSphereLight.h"
#include "Core_Actor.h"

Tools::Tools_Event<Core::Core_Actor&> Core::Core_Actor::DestroyedEvent;
Tools::Tools_Event<Core::Core_Actor&> Core::Core_Actor::CreatedEvent;
Tools::Tools_Event<Core::Core_Actor&, Core::Core_Actor&> Core::Core_Actor::AttachEvent;
Tools::Tools_Event<Core::Core_Actor&> Core::Core_Actor::DettachEvent;

Core::Core_Actor::Core_Actor(int64_t p_actorID, const std::string& p_name, const std::string& p_tag, bool& p_playing) :
	m_actorID(p_actorID),
	m_name(p_name),
	m_tag(p_tag),
	m_playing(p_playing),
	transform(AddComponent<Core_CTransform>())
{
	CreatedEvent.Invoke(*this);
}

Core::Core_Actor::~Core_Actor()
{
	if (!m_sleeping)
	{
		if (IsActive())
			OnDisable();

		if (m_awaked && m_started)
			OnDestroy();
	}

	DestroyedEvent.Invoke(*this);

	std::vector<Core_Actor*> toDetach = m_children;

	for (auto child : toDetach)
		child->DetachFromParent();

	toDetach.clear();

	DetachFromParent();

	std::for_each(m_components.begin(), m_components.end(), [&](std::shared_ptr<Core_AComponent> p_component) { ComponentRemovedEvent.Invoke(*p_component); });
	std::for_each(m_behaviours.begin(), m_behaviours.end(), [&](auto& p_behaviour) { BehaviourRemovedEvent.Invoke(std::ref(p_behaviour.second)); });
	std::for_each(m_children.begin(), m_children.end(), [](Core_Actor* p_element) { delete p_element; });
}

const std::string& Core::Core_Actor::GetName() const
{
	return m_name;
}

const std::string& Core::Core_Actor::GetTag() const
{
	return m_tag;
}

void Core::Core_Actor::SetName(const std::string& p_name)
{
	m_name = p_name;
}

void Core::Core_Actor::SetTag(const std::string& p_tag)
{
	m_tag = p_tag;
}

void Core::Core_Actor::SetActive(bool p_active)
{
	if (p_active != m_active)
	{
		RecursiveWasActiveUpdate();
		m_active = p_active;
		RecursiveActiveUpdate();
	}
}

bool Core::Core_Actor::IsSelfActive() const
{
	return m_active;
}

bool Core::Core_Actor::IsActive() const
{
	return m_active && (m_parent ? m_parent->IsActive() : true);
}

void Core::Core_Actor::SetID(int64_t p_id)
{
	m_actorID = p_id;
}

int64_t Core::Core_Actor::GetID() const
{
	return m_actorID;
}

void Core::Core_Actor::SetParent(Core_Actor& p_parent)
{
	DetachFromParent();

	m_parent = &p_parent;
	m_parentID = p_parent.m_actorID;
	transform.SetParent(p_parent.transform);

	p_parent.m_children.push_back(this);

	AttachEvent.Invoke(*this, p_parent);
}

void Core::Core_Actor::DetachFromParent()
{
	DettachEvent.Invoke(*this);

	if (m_parent)
	{
		m_parent->m_children.erase(std::remove_if(m_parent->m_children.begin(), m_parent->m_children.end(), [this](Core_Actor* p_element)
			{
				return p_element == this;
			}));
	}

	m_parent = nullptr;
	m_parentID = 0;

	transform.RemoveParent();
}

bool Core::Core_Actor::HasParent() const
{
	return m_parent;
}

Core::Core_Actor* Core::Core_Actor::GetParent() const
{
	return m_parent;
}

int64_t Core::Core_Actor::GetParentID() const
{
	return m_parentID;
}

std::vector<Core::Core_Actor*>& Core::Core_Actor::GetChildren()
{
	return m_children;
}

void Core::Core_Actor::MarkAsDestroy()
{
	m_destroyed = true;

	for (auto child : m_children)
		child->MarkAsDestroy();
}

bool Core::Core_Actor::IsAlive() const
{
	return !m_destroyed;
}

void Core::Core_Actor::SetSleeping(bool p_sleeping)
{
	m_sleeping = p_sleeping;
}

void Core::Core_Actor::OnAwake()
{
	m_awaked = true;
	std::for_each(m_components.begin(), m_components.end(), [](auto element) { element->OnAwake(); });
	std::for_each(m_behaviours.begin(), m_behaviours.end(), [](auto& element) { element.second.OnAwake(); });
}

void Core::Core_Actor::OnStart()
{
	m_started = true;
	std::for_each(m_components.begin(), m_components.end(), [](auto element) { element->OnStart(); });
	std::for_each(m_behaviours.begin(), m_behaviours.end(), [](auto& element) { element.second.OnStart(); });
}

void Core::Core_Actor::OnEnable()
{
	std::for_each(m_components.begin(), m_components.end(), [](auto element) { element->OnEnable(); });
	std::for_each(m_behaviours.begin(), m_behaviours.end(), [](auto& element) { element.second.OnEnable(); });
}

void Core::Core_Actor::OnDisable()
{
	std::for_each(m_components.begin(), m_components.end(), [](auto element) { element->OnDisable(); });
	std::for_each(m_behaviours.begin(), m_behaviours.end(), [](auto& element) { element.second.OnDisable(); });
}

void Core::Core_Actor::OnDestroy()
{
	std::for_each(m_components.begin(), m_components.end(), [](auto element) { element->OnDestroy(); });
	std::for_each(m_behaviours.begin(), m_behaviours.end(), [](auto& element) { element.second.OnDestroy(); });
}

void Core::Core_Actor::OnUpdate(float p_deltaTime)
{
	if (IsActive())
	{
		std::for_each(m_components.begin(), m_components.end(), [&](auto element) { element->OnUpdate(p_deltaTime); });
		std::for_each(m_behaviours.begin(), m_behaviours.end(), [&](auto& element) { element.second.OnUpdate(p_deltaTime); });
	}
}

void Core::Core_Actor::OnFixedUpdate(float p_deltaTime)
{
	if (IsActive())
	{
		std::for_each(m_components.begin(), m_components.end(), [&](auto element) { element->OnFixedUpdate(p_deltaTime); });
		std::for_each(m_behaviours.begin(), m_behaviours.end(), [&](auto& element) { element.second.OnFixedUpdate(p_deltaTime); });
	}
}

void Core::Core_Actor::OnLateUpdate(float p_deltaTime)
{
	if (IsActive())
	{
		std::for_each(m_components.begin(), m_components.end(), [&](auto element) { element->OnLateUpdate(p_deltaTime); });
		std::for_each(m_behaviours.begin(), m_behaviours.end(), [&](auto& element) { element.second.OnLateUpdate(p_deltaTime); });
	}
}

bool Core::Core_Actor::RemoveComponent(Core_AComponent& p_component)
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		if (it->get() == &p_component)
		{
			ComponentRemovedEvent.Invoke(p_component);
			m_components.erase(it);
			return true;
		}
	}

	return false;
}

std::vector<std::shared_ptr<Core::Core_AComponent>>& Core::Core_Actor::GetComponents()
{
	return m_components;
}

Core::Core_Behaviour& Core::Core_Actor::AddBehaviour(const std::string& p_name)
{
	m_behaviours.try_emplace(p_name, *this, p_name);
	Core_Behaviour& newInstance = m_behaviours.at(p_name);
	BehaviourAddedEvent.Invoke(newInstance);
	if (m_playing && IsActive())
	{
		newInstance.OnAwake();
		newInstance.OnEnable();
		newInstance.OnStart();
	}
	return newInstance;
}

bool Core::Core_Actor::RemoveBehaviour(Core_Behaviour& p_behaviour)
{
	bool found = false;

	for (auto& [name, behaviour] : m_behaviours)
	{
		if (&behaviour == &p_behaviour)
		{
			found = true;
			break;
		}
	}

	if (found)
		return RemoveBehaviour(p_behaviour.name);
	else
		return false;
}

bool Core::Core_Actor::RemoveBehaviour(const std::string& p_name)
{
	Core_Behaviour* found = GetBehaviour(p_name);
	if (found)
	{
		BehaviourRemovedEvent.Invoke(*found);
		return m_behaviours.erase(p_name);
	}
	else
	{
		return false;
	}
}

Core::Core_Behaviour* Core::Core_Actor::GetBehaviour(const std::string& p_name)
{
	if (auto result = m_behaviours.find(p_name); result != m_behaviours.end())
		return &result->second;
	else
		return nullptr;
}

std::unordered_map<std::string, Core::Core_Behaviour>& Core::Core_Actor::GetBehaviours()
{
	return m_behaviours;
}

void Core::Core_Actor::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot)
{
	tinyxml2::XMLNode* actorNode = p_doc.NewElement("actor");
	p_actorsRoot->InsertEndChild(actorNode);

	Core_Serializer::SerializeString(p_doc, actorNode, "name", m_name);
	Core_Serializer::SerializeString(p_doc, actorNode, "tag", m_tag);
	Core_Serializer::SerializeBoolean(p_doc, actorNode, "active", m_active);
	Core_Serializer::SerializeInt64(p_doc, actorNode, "id", m_actorID);
	Core_Serializer::SerializeInt64(p_doc, actorNode, "parent", m_parentID);

	tinyxml2::XMLNode* componentsNode = p_doc.NewElement("components");
	actorNode->InsertEndChild(componentsNode);

	for (auto& component : m_components)
	{
		tinyxml2::XMLNode* componentNode = p_doc.NewElement("component");
		componentsNode->InsertEndChild(componentNode);

		Core_Serializer::SerializeString(p_doc, componentNode, "type", typeid(*component).name());

		tinyxml2::XMLElement* data = p_doc.NewElement("data");
		componentNode->InsertEndChild(data);

		component->OnSerialize(p_doc, data);
	}

	tinyxml2::XMLNode* behavioursNode = p_doc.NewElement("behaviours");
	actorNode->InsertEndChild(behavioursNode);

	for (auto& behaviour : m_behaviours)
	{
		tinyxml2::XMLNode* behaviourNode = p_doc.NewElement("behaviour");
		behavioursNode->InsertEndChild(behaviourNode);

		Core_Serializer::SerializeString(p_doc, behaviourNode, "type", behaviour.first);

		tinyxml2::XMLElement* data = p_doc.NewElement("data");
		behaviourNode->InsertEndChild(data);

		behaviour.second.OnSerialize(p_doc, data);
	}
}

void Core::Core_Actor::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot)
{
	Core_Serializer::DeserializeString(p_doc, p_actorsRoot, "name", m_name);
	Core_Serializer::DeserializeString(p_doc, p_actorsRoot, "tag", m_tag);
	Core_Serializer::DeserializeBoolean(p_doc, p_actorsRoot, "active", m_active);
	Core_Serializer::DeserializeInt64(p_doc, p_actorsRoot, "id", m_actorID);
	Core_Serializer::DeserializeInt64(p_doc, p_actorsRoot, "parent", m_parentID);

	{
		tinyxml2::XMLNode* componentsRoot = p_actorsRoot->FirstChildElement("components");
		if (componentsRoot)
		{
			tinyxml2::XMLElement* currentComponent = componentsRoot->FirstChildElement("component");

			while (currentComponent)
			{
				std::string componentType = currentComponent->FirstChildElement("type")->GetText();
				Core_AComponent* component = nullptr;

				if (componentType == typeid(Core_CTransform).name())				component = &transform;
				else if (componentType == typeid(Core_CModelRenderer).name())		component = &AddComponent<Core_CModelRenderer>();
				else if (componentType == typeid(Core_CCamera).name())				component = &AddComponent<Core_CCamera>();
				else if (componentType == typeid(Core_CMaterialRenderer).name())	component = &AddComponent<Core_CMaterialRenderer>();
				else if (componentType == typeid(Core_CPointLight).name())			component = &AddComponent<Core_CPointLight>();
				else if (componentType == typeid(Core_CDirectionalLight).name())	component = &AddComponent<Core_CDirectionalLight>();
				else if (componentType == typeid(Core_CSpotLight).name())			component = &AddComponent<Core_CSpotLight>();
				else if (componentType == typeid(Core_CAmbientBoxLight).name())		component = &AddComponent<Core_CAmbientBoxLight>();
				else if (componentType == typeid(Core_CAmbientSphereLight).name())	component = &AddComponent<Core_CAmbientSphereLight>();

				if (component)
					component->OnDeserialize(p_doc, currentComponent->FirstChildElement("data"));

				currentComponent = currentComponent->NextSiblingElement("component");
			}
		}
	}

	{
		tinyxml2::XMLNode* behavioursRoot = p_actorsRoot->FirstChildElement("behaviours");

		if (behavioursRoot)
		{
			tinyxml2::XMLElement* currentBehaviour = behavioursRoot->FirstChildElement("behaviour");

			while (currentBehaviour)
			{
				std::string behaviourType = currentBehaviour->FirstChildElement("type")->GetText();

				auto& behaviour = AddBehaviour(behaviourType);
				behaviour.OnDeserialize(p_doc, currentBehaviour->FirstChildElement("data"));

				currentBehaviour = currentBehaviour->NextSiblingElement("behaviour");
			}
		}
	}
}

void Core::Core_Actor::RecursiveActiveUpdate()
{
	bool isActive = IsActive();

	if (!m_sleeping)
	{
		if (!m_wasActive && isActive)
		{
			if (!m_awaked)
				OnAwake();

			OnEnable();

			if (!m_started)
				OnStart();
		}

		if (m_wasActive && !isActive)
			OnDisable();
	}

	for (auto child : m_children)
		child->RecursiveActiveUpdate();
}

void Core::Core_Actor::RecursiveWasActiveUpdate()
{
	m_wasActive = IsActive();
	for (auto child : m_children)
		child->RecursiveWasActiveUpdate();
}
