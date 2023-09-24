#pragma once

#include "Core_AComponent.h"

namespace Core
{
	class Core_Actor;

	class Core_Behaviour : public Core_AComponent
	{
	public:
		Core_Behaviour(Core_Actor& pOwner, const std::string& pName);

		~Core_Behaviour();

		virtual std::string GetName() override;

		//bool RegisterToLuaContext(sol::state& p_luaState, const std::string& pScriptFolder);

		//void UnregisterFromLuaContext();

		//template<typename... Args>
		//void LuaCall(const std::string& p_functionName, Args&&... pArgs);

		//sol::table& GetTable();

		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnEnable() override;

		virtual void OnDisable() override;

		virtual void OnDestroy() override;

		virtual void OnUpdate(float pDeltaTime) override;

		virtual void OnFixedUpdate(float pDeltaTime) override;

		virtual void OnLateUpdate(float pDeltaTime) override;

		virtual void OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode) override;

		virtual void OnInspector(UI::UI_WidgetContainer& pRoot) override;

	public:
		static Tools::Tools_Event<Core_Behaviour*> mCreatedEvent;
		static Tools::Tools_Event<Core_Behaviour*> mDestroyedEvent;

		const std::string name;

	private:
		//sol::table m_object = sol::nil;
	};
}

#include "Core_Behaviour.inl"