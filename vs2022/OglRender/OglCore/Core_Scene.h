#pragma once

#include "Core_Actor.h"
#include "Core_ISerializable.h"

#include "Core_CModelRenderer.h"
#include "Core_CCamera.h"
#include "Core_CLight.h"

namespace Core
{
	class Core_Scene : public Core_ISerializable
	{
	public:
		struct FastAccessComponents
		{
			std::vector<Core_CModelRenderer*>	modelRenderers;
			std::vector<Core_CCamera*>			cameras;
			std::vector<Core_CLight*>			lights;
		};

		Core_Scene();

		~Core_Scene();

		void Play();

		bool IsPlaying() const;

		void Update(float pDeltaTime);

		void FixedUpdate(float pDeltaTime);

		void LateUpdate(float pDeltaTime);

		Core_Actor& CreateActor();

		Core_Actor& CreateActor(const std::string& pName, const std::string& pTag = "");

		bool DestroyActor(Core_Actor& pTarget);

		void CollectGarbages();

		Core_Actor* FindActorByName(const std::string& pName);

		Core_Actor* FindActorByTag(const std::string& pTag);

		Core_Actor* FindActorByID(int64_t pId);

		std::vector<std::reference_wrapper<Core_Actor>> FindActorsByName(const std::string& pName);

		std::vector<std::reference_wrapper<Core_Actor>> FindActorsByTag(const std::string& pTag);

		void OnComponentAdded(Core_AComponent& pCompononent);

		void OnComponentRemoved(Core_AComponent& pCompononent);

		std::vector<Core_Actor*>& GetActors();

		const FastAccessComponents& GetFastAccessComponents() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pRoot) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pRoot) override;

	private:
		int64_t mAvailableID{ 1 };
		bool mIsPlaying{ false };
		std::vector<Core_Actor*> mActors;

		FastAccessComponents mFastAccessComponents;
	};
}

