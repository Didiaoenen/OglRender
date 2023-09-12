#include <OglUI/UI_TextColored.h>

#include "Core_Actor.h"
#include "Core_Behaviour.h"

Tools::Tools_Event<Core::Core_Behaviour*> Core::Core_Behaviour::mCreatedEvent;
Tools::Tools_Event<Core::Core_Behaviour*> Core::Core_Behaviour::mDestroyedEvent;

Core::Core_Behaviour::Core_Behaviour(Core_Actor& pOwner, const std::string& pName) :
	name(pName), Core_AComponent(pOwner)
{
	mCreatedEvent.Invoke(this);
}

Core::Core_Behaviour::~Core_Behaviour()
{
	mDestroyedEvent.Invoke(this);
}

std::string Core::Core_Behaviour::GetName()
{
	return "Behaviour";
}

void Core::Core_Behaviour::OnAwake()
{
}

void Core::Core_Behaviour::OnStart()
{
}

void Core::Core_Behaviour::OnEnable()
{
}

void Core::Core_Behaviour::OnDisable()
{
}

void Core::Core_Behaviour::OnDestroy()
{
}

void Core::Core_Behaviour::OnUpdate(float pDeltaTime)
{
}

void Core::Core_Behaviour::OnFixedUpdate(float pDeltaTime)
{
}

void Core::Core_Behaviour::OnLateUpdate(float pDeltaTime)
{
}

void Core::Core_Behaviour::OnSerialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
}

void Core::Core_Behaviour::OnDeserialize(tinyxml2::XMLDocument& pDoc, tinyxml2::XMLNode* pNode)
{
}

void Core::Core_Behaviour::OnInspector(UI::UI_WidgetContainer& pRoot)
{
}
