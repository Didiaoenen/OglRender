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
		Core_Actor(int64_t p_actorID, const std::string& p_name, const std::string& p_tag, bool& p_playing);

		virtual ~Core_Actor() override;

		const std::string& GetName() const;

		const std::string& GetTag() const;

		void SetName(const std::string& p_name);

		void SetTag(const std::string& p_tag);

		void SetActive(bool p_active);

		bool IsSelfActive() const;

		bool IsActive() const;

		void SetID(int64_t p_id);

		int64_t GetID() const;

		void SetParent(Core_Actor& p_parent);

		void DetachFromParent();

		bool HasParent() const;

		Core_Actor* GetParent() const;

		int64_t GetParentID() const;

		std::vector<Core_Actor*>& GetChildren();

		void MarkAsDestroy();

		bool IsAlive() const;

		void SetSleeping(bool p_sleeping);

		void OnAwake();

		void OnStart();

		void OnEnable();

		void OnDisable();

		void OnDestroy();

		void OnUpdate(float p_deltaTime);

		void OnFixedUpdate(float p_deltaTime);

		void OnLateUpdate(float p_deltaTime);

		//void OnCollisionEnter(CPhysicalObject& p_otherObject);

		//void OnCollisionStay(CPhysicalObject& p_otherObject);

		//void OnCollisionExit(CPhysicalObject& p_otherObject);

		//void OnTriggerEnter(CPhysicalObject& p_otherObject);

		//void OnTriggerStay(CPhysicalObject& p_otherObject);

		//void OnTriggerExit(CPhysicalObject& p_otherObject);

		template<typename T, typename ... Args>
		T& AddComponent(Args&&... p_args);

		template<typename T>
		bool RemoveComponent();

		bool RemoveComponent(Core_AComponent& p_component);

		template<typename T>
		T* GetComponent();

		std::vector<std::shared_ptr<Core_AComponent>>& GetComponents();

		Core_Behaviour& AddBehaviour(const std::string& p_name);

		bool RemoveBehaviour(Core_Behaviour& p_behaviour);

		bool RemoveBehaviour(const std::string& p_name);

		Core_Behaviour* GetBehaviour(const std::string& p_name);

		std::unordered_map<std::string, Core_Behaviour>& GetBehaviours();

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot) override;

	private:
		Core_Actor(const Core_Actor& p_actor) = delete;

		void RecursiveActiveUpdate();
		void RecursiveWasActiveUpdate();

	public:
		Tools::Tools_Event<Core_AComponent&> ComponentAddedEvent;
		Tools::Tools_Event<Core_AComponent&> ComponentRemovedEvent;
		Tools::Tools_Event<Core_Behaviour&>	BehaviourAddedEvent;
		Tools::Tools_Event<Core_Behaviour&>	BehaviourRemovedEvent;

		static Tools::Tools_Event<Core_Actor&>				DestroyedEvent;
		static Tools::Tools_Event<Core_Actor&>				CreatedEvent;
		static Tools::Tools_Event<Core_Actor&, Core_Actor&>	AttachEvent;
		static Tools::Tools_Event<Core_Actor&>				DettachEvent;

	private:
		std::string		m_name;
		std::string		m_tag;
		bool			m_active = true;
		bool& m_playing;

		int64_t	m_actorID;
		bool	m_destroyed = false;
		bool	m_sleeping = true;
		bool	m_awaked = false;
		bool	m_started = false;
		bool	m_wasActive = false;

		int64_t		m_parentID = 0;
		Core_Actor* m_parent = nullptr;
		std::vector<Core_Actor*> m_children;

		std::vector<std::shared_ptr<Core_AComponent>> m_components;
		std::unordered_map<std::string, Core_Behaviour> m_behaviours;

	public:
		Core_CTransform& transform;
	};
}

#include "Core_Actor.inl"