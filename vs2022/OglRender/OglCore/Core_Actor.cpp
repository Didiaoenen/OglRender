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

Tools::Tools_Event<Core::Core_Actor&> Core::Core_Actor::mDestroyedEvent;
Tools::Tools_Event<Core::Core_Actor&> Core::Core_Actor::mCreatedEvent;
Tools::Tools_Event<Core::Core_Actor&, Core::Core_Actor&> Core::Core_Actor::mAttachEvent;
Tools::Tools_Event<Core::Core_Actor&> Core::Core_Actor::mDettachEvent;

Core::Core_Actor::Core_Actor(int64_t pActorID, const std::string& pName, const std::string& pTag, bool& pPlaying) :
	mActorID(pActorID),
	mName(pName),
	mTag(pTag),
	mPlaying(pPlaying),
	transform(AddComponent<Core_CTransform>())
{
	mCreatedEvent.Invoke(*this);
}

Core::Core_Actor::~Core_Actor()
{
	if (!mSleeping)
	{
		if (IsActive())
		{
			OnDisable();
		}

		if (mAwaked && mStarted)
		{
			OnDestroy();
		}
	}

	mDestroyedEvent.Invoke(*this);

	std::vector<Core_Actor*> toDetach = mChildren;

	for (auto child : toDetach)
	{
		child->DetachFromParent();
	}

	toDetach.clear();

	DetachFromParent();

	std::for_each(mComponents.begin(), mComponents.end(), [&](std::shared_ptr<Core_AComponent> pComponent) { mComponentRemovedEvent.Invoke(*pComponent); });
	std::for_each(mBehaviours.begin(), mBehaviours.end(), [&](auto& pBehaviour) { mBehaviourRemovedEvent.Invoke(std::ref(pBehaviour.second)); });
	std::for_each(mChildren.begin(), mChildren.end(), [](Core_Actor* pElement) { delete pElement; });
}

const std::string& Core::Core_Actor::GetName() const
{
	return mName;
}

const std::string& Core::Core_Actor::GetTag() const
{
	return mTag;
}

void Core::Core_Actor::SetName(const std::string& pName)
{
	mName = pName;
}

void Core::Core_Actor::SetTag(const std::string& pTag)
{
	mTag = pTag;
}

void Core::Core_Actor::SetActive(bool pActive)
{
	if (pActive != mActive)
	{
		RecursiveWasActiveUpdate();
		mActive = pActive;
		RecursiveActiveUpdate();
	}
}

bool Core::Core_Actor::IsSelfActive() const
{
	return mActive;
}

bool Core::Core_Actor::IsActive() const
{
	return mActive && (mParent ? mParent->IsActive() : true);
}

void Core::Core_Actor::SetID(int64_t pId)
{
	mActorID = pId;
}

int64_t Core::Core_Actor::GetID() const
{
	return mActorID;
}

void Core::Core_Actor::SetParent(Core_Actor& pParent)
{
	DetachFromParent();

	mParent = &pParent;
	mParentID = pParent.mActorID;
	transform.SetParent(pParent.transform);

	pParent.mChildren.push_back(this);

	mAttachEvent.Invoke(*this, pParent);
}

void Core::Core_Actor::DetachFromParent()
{
	mDettachEvent.Invoke(*this);

	if (mParent)
	{
		mParent->mChildren.erase(std::remove_if(mParent->mChildren.begin(), mParent->mChildren.end(), [this](Core_Actor* pElement)
			{
				return pElement == this;
			}));
	}

	mParent = nullptr;
	mParentID = 0;

	transform.RemoveParent();
}

bool Core::Core_Actor::HasParent() const
{
	return mParent;
}

Core::Core_Actor* Core::Core_Actor::GetParent() const
{
	return mParent;
}

int64_t Core::Core_Actor::GetParentID() const
{
	return mParentID;
}

std::vector<Core::Core_Actor*>& Core::Core_Actor::GetChildren()
{
	return mChildren;
}

void Core::Core_Actor::MarkAsDestroy()
{
	mDestroyed = true;

	for (auto child : mChildren)
	{
		child->MarkAsDestroy();
	}
}

bool Core::Core_Actor::IsAlive() const
{
	return !mDestroyed;
}

void Core::Core_Actor::SetSleeping(bool pSleeping)
{
	mSleeping = pSleeping;
}

void Core::Core_Actor::OnAwake()
{
	mAwaked = true;
	std::for_each(mComponents.begin(), mComponents.end(), [](auto pElement) { pElement->OnAwake(); });
	std::for_each(mBehaviours.begin(), mBehaviours.end(), [](auto& pElement) { pElement.second.OnAwake(); });
}

void Core::Core_Actor::OnStart()
{
	mStarted = true;
	std::for_each(mComponents.begin(), mComponents.end(), [](auto pElement) { pElement->OnStart(); });
	std::for_each(mBehaviours.begin(), mBehaviours.end(), [](auto& pElement) { pElement.second.OnStart(); });
}

void Core::Core_Actor::OnEnable()
{
	std::for_each(mComponents.begin(), mComponents.end(), [](auto pElement) { pElement->OnEnable(); });
	std::for_each(mBehaviours.begin(), mBehaviours.end(), [](auto& pElement) { pElement.second.OnEnable(); });
}

void Core::Core_Actor::OnDisable()
{
	std::for_each(mComponents.begin(), mComponents.end(), [](auto pElement) { pElement->OnDisable(); });
	std::for_each(mBehaviours.begin(), mBehaviours.end(), [](auto& pElement) { pElement.second.OnDisable(); });
}

void Core::Core_Actor::OnDestroy()
{
	std::for_each(mComponents.begin(), mComponents.end(), [](auto pElement) { pElement->OnDestroy(); });
	std::for_each(mBehaviours.begin(), mBehaviours.end(), [](auto& pElement) { pElement.second.OnDestroy(); });
}

void Core::Core_Actor::OnUpdate(float pDeltaTime)
{
	if (IsActive())
	{
		std::for_each(mComponents.begin(), mComponents.end(), [&](auto pElement) { pElement->OnUpdate(pDeltaTime); });
		std::for_each(mBehaviours.begin(), mBehaviours.end(), [&](auto& pElement) { pElement.second.OnUpdate(pDeltaTime); });
	}
}

void Core::Core_Actor::OnFixedUpdate(float pDeltaTime)
{
	if (IsActive())
	{
		std::for_each(mComponents.begin(), mComponents.end(), [&](auto pElement) { pElement->OnFixedUpdate(pDeltaTime); });
		std::for_each(mBehaviours.begin(), mBehaviours.end(), [&](auto& pElement) { pElement.second.OnFixedUpdate(pDeltaTime); });
	}
}

void Core::Core_Actor::OnLateUpdate(float pDeltaTime)
{
	if (IsActive())
	{
		std::for_each(mComponents.begin(), mComponents.end(), [&](auto pElement) { pElement->OnLateUpdate(pDeltaTime); });
		std::for_each(mBehaviours.begin(), mBehaviours.end(), [&](auto& pElement) { pElement.second.OnLateUpdate(pDeltaTime); });
	}
}

bool Core::Core_Actor::RemoveComponent(Core_AComponent& pComponent)
{
	for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		if (it->get() == &pComponent)
		{
			mComponentRemovedEvent.Invoke(pComponent);
			mComponents.erase(it);
			return true;
		}
	}

	return false;
}

std::vector<std::shared_ptr<Core::Core_AComponent>>& Core::Core_Actor::GetComponents()
{
	return mComponents;
}

Core::Core_Behaviour& Core::Core_Actor::AddBehaviour(const std::string& pName)
{
	mBehaviours.try_emplace(pName, *this, pName);
	Core_Behaviour& newInstance = mBehaviours.at(pName);
	mBehaviourAddedEvent.Invoke(newInstance);
	if (mPlaying && IsActive())
	{
		newInstance.OnAwake();
		newInstance.OnEnable();
		newInstance.OnStart();
	}
	return newInstance;
}

bool Core::Core_Actor::RemoveBehaviour(Core_Behaviour& pBehaviour)
{
	bool found = false;

	for (auto& [name, behaviour] : mBehaviours)
	{
		if (&behaviour == &pBehaviour)
		{
			found = true;
			break;
		}
	}

	if (found)
	{
		return RemoveBehaviour(pBehaviour.name);
	}
	else
	{
		return false;
	}
}

bool Core::Core_Actor::RemoveBehaviour(const std::string& pName)
{
	Core_Behaviour* found = GetBehaviour(pName);
	if (found)
	{
		mBehaviourRemovedEvent.Invoke(*found);
		return mBehaviours.erase(pName);
	}
	else
	{
		return false;
	}
}

Core::Core_Behaviour* Core::Core_Actor::GetBehaviour(const std::string& pName)
{
	if (auto result = mBehaviours.find(pName); result != mBehaviours.end())
	{
		return &result->second;
	}
	else
	{
		return nullptr;
	}
}

std::unordered_map<std::string, Core::Core_Behaviour>& Core::Core_Actor::GetBehaviours()
{
	return mBehaviours;
}

void Core::Core_Actor::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pActorsRoot)
{
	tinyxml2::XMLNode* actorNode = pDoc.NewElement("actor");
	pActorsRoot->InsertEndChild(actorNode);

	Core_Serializer::SerializeString(pDoc, actorNode, "name", mName);
	Core_Serializer::SerializeString(pDoc, actorNode, "tag", mTag);
	Core_Serializer::SerializeBoolean(pDoc, actorNode, "active", mActive);
	Core_Serializer::SerializeInt64(pDoc, actorNode, "id", mActorID);
	Core_Serializer::SerializeInt64(pDoc, actorNode, "parent", mParentID);

	tinyxml2::XMLNode* componentsNode = pDoc.NewElement("components");
	actorNode->InsertEndChild(componentsNode);

	for (auto& component : mComponents)
	{
		tinyxml2::XMLNode* componentNode = pDoc.NewElement("component");
		componentsNode->InsertEndChild(componentNode);

		Core_Serializer::SerializeString(pDoc, componentNode, "type", typeid(*component).name());

		tinyxml2::XMLElement* data = pDoc.NewElement("data");
		componentNode->InsertEndChild(data);

		component->OnSerialize(pDoc, data);
	}

	tinyxml2::XMLNode* behavioursNode = pDoc.NewElement("behaviours");
	actorNode->InsertEndChild(behavioursNode);

	for (auto& behaviour : mBehaviours)
	{
		tinyxml2::XMLNode* behaviourNode = pDoc.NewElement("behaviour");
		behavioursNode->InsertEndChild(behaviourNode);

		Core_Serializer::SerializeString(pDoc, behaviourNode, "type", behaviour.first);

		tinyxml2::XMLElement* data = pDoc.NewElement("data");
		behaviourNode->InsertEndChild(data);

		behaviour.second.OnSerialize(pDoc, data);
	}
}

void Core::Core_Actor::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pActorsRoot)
{
	Core_Serializer::DeserializeString(pDoc, pActorsRoot, "name", mName);
	Core_Serializer::DeserializeString(pDoc, pActorsRoot, "tag", mTag);
	Core_Serializer::DeserializeBoolean(pDoc, pActorsRoot, "active", mActive);
	Core_Serializer::DeserializeInt64(pDoc, pActorsRoot, "id", mActorID);
	Core_Serializer::DeserializeInt64(pDoc, pActorsRoot, "parent", mParentID);

	{
		tinyxml2::XMLNode* componentsRoot = pActorsRoot->FirstChildElement("components");
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
				{
					component->OnDeserialize(pDoc, currentComponent->FirstChildElement("data"));
				}

				currentComponent = currentComponent->NextSiblingElement("component");
			}
		}
	}

	{
		tinyxml2::XMLNode* behavioursRoot = pActorsRoot->FirstChildElement("behaviours");

		if (behavioursRoot)
		{
			tinyxml2::XMLElement* currentBehaviour = behavioursRoot->FirstChildElement("behaviour");

			while (currentBehaviour)
			{
				std::string behaviourType = currentBehaviour->FirstChildElement("type")->GetText();

				auto& behaviour = AddBehaviour(behaviourType);
				behaviour.OnDeserialize(pDoc, currentBehaviour->FirstChildElement("data"));

				currentBehaviour = currentBehaviour->NextSiblingElement("behaviour");
			}
		}
	}
}

void Core::Core_Actor::RecursiveActiveUpdate()
{
	bool isActive = IsActive();

	if (!mSleeping)
	{
		if (!mWasActive && isActive)
		{
			if (!mAwaked)
			{
				OnAwake();
			}

			OnEnable();

			if (!mStarted)
			{
				OnStart();
			}
		}

		if (mWasActive && !isActive)
		{
			OnDisable();
		}
	}

	for (auto child : mChildren)
	{
		child->RecursiveActiveUpdate();
	}
}

void Core::Core_Actor::RecursiveWasActiveUpdate()
{
	mWasActive = IsActive();
	for (auto child : mChildren)
	{
		child->RecursiveWasActiveUpdate();
	}
}
