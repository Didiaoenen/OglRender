#pragma once

#include <memory>
#include <unordered_map>

#include <OglTools/Tools_Event.h>

#include "Core_AComponent.h"
#include "Core_CTransform.h"
#include "Core_Behaviour.h"
#include "Core_ISerializable.h"

namespace Core
{
	class Core_Actor : public Core_ISerializable
	{
	public:
		Core_Actor(int64_t pActorID, const std::string& pName, const std::string& pTag, bool& pPlaying);

		virtual ~Core_Actor() override;

		const std::string& GetName() const;

		const std::string& GetTag() const;

		void SetName(const std::string& pName);

		void SetTag(const std::string& pTag);

		void SetActive(bool pActive);

		bool IsSelfActive() const;

		bool IsActive() const;

		void SetID(int64_t pId);

		int64_t GetID() const;

		void SetParent(Core_Actor& pParent);

		void DetachFromParent();

		bool HasParent() const;

		Core_Actor* GetParent() const;

		int64_t GetParentID() const;

		std::vector<Core_Actor*>& GetChildren();

		void MarkAsDestroy();

		bool IsAlive() const;

		void SetSleeping(bool pSleeping);

		void OnAwake();

		void OnStart();

		void OnEnable();

		void OnDisable();

		void OnDestroy();

		void OnUpdate(float pDeltaTime);

		void OnFixedUpdate(float pDeltaTime);

		void OnLateUpdate(float pDeltaTime);

		template<typename T, typename ... Args>
		T& AddComponent(Args&&... pArgs);

		template<typename T>
		bool RemoveComponent();

		bool RemoveComponent(Core_AComponent& pComponent);

		template<typename T>
		T* GetComponent();

		std::vector<std::shared_ptr<Core_AComponent>>& GetComponents();

		Core_Behaviour& AddBehaviour(const std::string& pName);

		bool RemoveBehaviour(Core_Behaviour& pBehaviour);

		bool RemoveBehaviour(const std::string& pName);

		Core_Behaviour* GetBehaviour(const std::string& pName);

		std::unordered_map<std::string, Core_Behaviour>& GetBehaviours();

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pActorsRoot) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pActorsRoot) override;

	private:
		Core_Actor(const Core_Actor& pActor) = delete;

		void RecursiveActiveUpdate();
		void RecursiveWasActiveUpdate();

	public:
		Tools::Tools_Event<Core_AComponent&> mComponentAddedEvent;
		Tools::Tools_Event<Core_AComponent&> mComponentRemovedEvent;
		Tools::Tools_Event<Core_Behaviour&>	mBehaviourAddedEvent;
		Tools::Tools_Event<Core_Behaviour&>	mBehaviourRemovedEvent;

		static Tools::Tools_Event<Core_Actor&>				mDestroyedEvent;
		static Tools::Tools_Event<Core_Actor&>				mCreatedEvent;
		static Tools::Tools_Event<Core_Actor&, Core_Actor&>	mAttachEvent;
		static Tools::Tools_Event<Core_Actor&>				mDettachEvent;

	private:
		std::string		mName{ "" };
		std::string		mTag{ "" };
		bool			mActive{ true };
		bool&			mPlaying;

		int64_t	mActorID{ 0 };
		bool	mDestroyed{ false };
		bool	mSleeping{ true };
		bool	mAwaked{ false };
		bool	mStarted{ false };
		bool	mWasActive{ false };

		int64_t		mParentID{ 0 };
		Core_Actor* mParent{ nullptr };
		std::vector<Core_Actor*> mChildren;

		std::vector<std::shared_ptr<Core_AComponent>> mComponents;
		std::unordered_map<std::string, Core_Behaviour> mBehaviours;

	public:
		Core_CTransform& transform;
	};
}

#include "Core_Actor.inl"