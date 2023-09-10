#include <OglUI/UI_TextColored.h>

#include "Core_Actor.h"
#include "Core_Behaviour.h"

Tools::Tools_Event<Core::Core_Behaviour*> Core::Core_Behaviour::CreatedEvent;
Tools::Tools_Event<Core::Core_Behaviour*> Core::Core_Behaviour::DestroyedEvent;

Core::Core_Behaviour::Core_Behaviour(Core_Actor& p_owner, const std::string& p_name) :
	name(p_name), Core_AComponent(p_owner)
{
	CreatedEvent.Invoke(this);
}

Core::Core_Behaviour::~Core_Behaviour()
{
	DestroyedEvent.Invoke(this);
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

void Core::Core_Behaviour::OnUpdate(float p_deltaTime)
{
}

void Core::Core_Behaviour::OnFixedUpdate(float p_deltaTime)
{
}

void Core::Core_Behaviour::OnLateUpdate(float p_deltaTime)
{
}

void Core::Core_Behaviour::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void Core::Core_Behaviour::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void Core::Core_Behaviour::OnInspector(UI::UI_WidgetContainer& p_root)
{
}
