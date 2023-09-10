#pragma once

#include "Core_AComponent.h"

namespace Core
{
	class Core_Actor;

	class Core_Behaviour : public Core_AComponent
	{
	public:
		Core_Behaviour(Core_Actor& p_owner, const std::string& p_name);

		~Core_Behaviour();

		virtual std::string GetName() override;

		//bool RegisterToLuaContext(sol::state& p_luaState, const std::string& p_scriptFolder);

		//void UnregisterFromLuaContext();

		//template<typename... Args>
		//void LuaCall(const std::string& p_functionName, Args&&... p_args);

		//sol::table& GetTable();

		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnEnable() override;

		virtual void OnDisable() override;

		virtual void OnDestroy() override;

		virtual void OnUpdate(float p_deltaTime) override;

		virtual void OnFixedUpdate(float p_deltaTime) override;

		virtual void OnLateUpdate(float p_deltaTime) override;

		//virtual void OnCollisionEnter(Core_CPhysicalObject& p_otherObject) override;

		//virtual void OnCollisionStay(Core_CPhysicalObject& p_otherObject) override;

		//virtual void OnCollisionExit(Core_CPhysicalObject& p_otherObject) override;

		//virtual void OnTriggerEnter(Core_CPhysicalObject& p_otherObject) override;

		//virtual void OnTriggerStay(Core_CPhysicalObject& p_otherObject) override;

		//virtual void OnTriggerExit(Core_CPhysicalObject& p_otherObject) override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::UI_WidgetContainer& p_root) override;

	public:
		static Tools::Tools_Event<Core_Behaviour*> CreatedEvent;
		static Tools::Tools_Event<Core_Behaviour*> DestroyedEvent;

		const std::string name;

	private:
		//sol::table m_object = sol::nil;
	};
}

#include "Core_Behaviour.inl"