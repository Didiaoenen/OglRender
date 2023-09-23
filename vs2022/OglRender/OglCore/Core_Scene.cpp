#include <string>
#include <algorithm>

#include "Core_Scene.h"

Core::Core_Scene::Core_Scene()
{
}

Core::Core_Scene::~Core_Scene()
{
	std::for_each(mActors.begin(), mActors.end(), [](Core_Actor* pElement)
		{
			delete pElement;
		});

	mActors.clear();
}

void Core::Core_Scene::Play()
{
	mIsPlaying = true;

	std::for_each(mActors.begin(), mActors.end(), [](Core_Actor* pElement) { pElement->SetSleeping(false); });

	std::for_each(mActors.begin(), mActors.end(), [](Core_Actor* pElement) { if (pElement->IsActive()) pElement->OnAwake(); });
	std::for_each(mActors.begin(), mActors.end(), [](Core_Actor* pElement) { if (pElement->IsActive()) pElement->OnEnable(); });
	std::for_each(mActors.begin(), mActors.end(), [](Core_Actor* pElement) { if (pElement->IsActive()) pElement->OnStart(); });
}

bool Core::Core_Scene::IsPlaying() const
{
	return mIsPlaying;
}

void Core::Core_Scene::Update(float pDeltaTime)
{
	auto actors = mActors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core_Actor::OnUpdate), std::placeholders::_1, pDeltaTime));
}

void Core::Core_Scene::FixedUpdate(float pDeltaTime)
{
	auto actors = mActors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core_Actor::OnFixedUpdate), std::placeholders::_1, pDeltaTime));
}

void Core::Core_Scene::LateUpdate(float pDeltaTime)
{
	auto actors = mActors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Core_Actor::OnLateUpdate), std::placeholders::_1, pDeltaTime));
}

Core::Core_Actor& Core::Core_Scene::CreateActor()
{
	return CreateActor("New Actor");
}

Core::Core_Actor& Core::Core_Scene::CreateActor(const std::string& pName, const std::string& pTag)
{
	mActors.push_back(new Core_Actor(mAvailableID++, pName, pTag, mIsPlaying));
	Core_Actor& instance = *mActors.back();
	instance.mComponentAddedEvent += std::bind(&Core_Scene::OnComponentAdded, this, std::placeholders::_1);
	instance.mComponentRemovedEvent += std::bind(&Core_Scene::OnComponentRemoved, this, std::placeholders::_1);
	if (mIsPlaying)
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
	auto found = std::find_if(mActors.begin(), mActors.end(), [&pTarget](Core_Actor* pElement)
		{
			return pElement == &pTarget;
		});

	if (found != mActors.end())
	{
		delete* found;
		mActors.erase(found);
		return true;
	}
	else
	{
		return false;
	}
}

void Core::Core_Scene::CollectGarbages()
{
	mActors.erase(std::remove_if(mActors.begin(), mActors.end(), [this](Core_Actor* pElement)
		{
			bool isGarbage = !pElement->IsAlive();
			if (isGarbage)
			{
				delete pElement;
			}
			return isGarbage;
		}), mActors.end());
}

Core::Core_Actor* Core::Core_Scene::FindActorByName(const std::string& pName)
{
	auto result = std::find_if(mActors.begin(), mActors.end(), [pName](Core_Actor* pElement)
		{
			return pElement->GetName() == pName;
		});

	if (result != mActors.end())
	{
		return *result;
	}
	else
	{
		return nullptr;
	}
}

Core::Core_Actor* Core::Core_Scene::FindActorByTag(const std::string& pTag)
{
	auto result = std::find_if(mActors.begin(), mActors.end(), [pTag](Core_Actor* pElement)
		{
			return pElement->GetTag() == pTag;
		});

	if (result != mActors.end())
	{
		return *result;
	}
	else
	{
		return nullptr;
	}
}

Core::Core_Actor* Core::Core_Scene::FindActorByID(int64_t pId)
{
	auto result = std::find_if(mActors.begin(), mActors.end(), [pId](Core_Actor* pElement)
		{
			return pElement->GetID() == pId;
		});

	if (result != mActors.end())
	{
		return *result;
	}
	else
	{
		return nullptr;
	}
}

std::vector<std::reference_wrapper<Core::Core_Actor>> Core::Core_Scene::FindActorsByName(const std::string& pName)
{
	std::vector<std::reference_wrapper<Core_Actor>> actors;

	for (auto actor : mActors)
	{
		if (actor->GetName() == pName)
		{
			actors.push_back(std::ref(*actor));
		}
	}

	return actors;
}

std::vector<std::reference_wrapper<Core::Core_Actor>> Core::Core_Scene::FindActorsByTag(const std::string& pTag)
{
	std::vector<std::reference_wrapper<Core_Actor>> actors;

	for (auto actor : mActors)
	{
		if (actor->GetTag() == pTag)
		{
			actors.push_back(std::ref(*actor));
		}
	}

	return actors;
}

void Core::Core_Scene::OnComponentAdded(Core_AComponent& pCompononent)
{
	if (auto result = dynamic_cast<Core_CModelRenderer*>(&pCompononent))
	{
		mFastAccessComponents.modelRenderers.push_back(result);
	}

	if (auto result = dynamic_cast<Core_CCamera*>(&pCompononent))
	{
		mFastAccessComponents.cameras.push_back(result);
	}

	if (auto result = dynamic_cast<Core_CLight*>(&pCompononent))
	{
		mFastAccessComponents.lights.push_back(result);
	}
}

void Core::Core_Scene::OnComponentRemoved(Core_AComponent& pCompononent)
{
	if (auto result = dynamic_cast<Core_CModelRenderer*>(&pCompononent))
	{
		mFastAccessComponents.modelRenderers.erase(std::remove(mFastAccessComponents.modelRenderers.begin(), mFastAccessComponents.modelRenderers.end(), result), mFastAccessComponents.modelRenderers.end());
	}

	if (auto result = dynamic_cast<Core_CCamera*>(&pCompononent))
	{
		mFastAccessComponents.cameras.erase(std::remove(mFastAccessComponents.cameras.begin(), mFastAccessComponents.cameras.end(), result), mFastAccessComponents.cameras.end());
	}

	if (auto result = dynamic_cast<Core_CLight*>(&pCompononent))
	{
		mFastAccessComponents.lights.erase(std::remove(mFastAccessComponents.lights.begin(), mFastAccessComponents.lights.end(), result), mFastAccessComponents.lights.end());
	}
}

std::vector<Core::Core_Actor*>& Core::Core_Scene::GetActors()
{
	return mActors;
}

const Core::Core_Scene::FastAccessComponents& Core::Core_Scene::GetFastAccessComponents() const
{
	return mFastAccessComponents;
}

void Core::Core_Scene::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pRoot)
{
	tinyxml2::XMLNode* sceneNode = pDoc.NewElement("scene");
	pRoot->InsertEndChild(sceneNode);

	tinyxml2::XMLNode* actorsNode = pDoc.NewElement("actors");
	sceneNode->InsertEndChild(actorsNode);

	for (auto& actor : mActors)
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

		mAvailableID = maxID;

		for (auto actor : mActors)
		{
			if (actor->GetParentID() > 0)
			{
				if (auto found = FindActorByID(actor->GetParentID()); found)
				{
					actor->SetParent(*found);
				}
			}
		}
	}
}
