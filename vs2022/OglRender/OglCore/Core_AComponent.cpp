#include "Core_Actor.h"
#include "Core_AComponent.h"

Core::Core_AComponent::Core_AComponent(Core_Actor& p_owner)
	: owner(p_owner)
{
}

Core::Core_AComponent::~Core_AComponent()
{
	if (owner.IsActive())
	{
		OnDisable();
		OnDestroy();
	}
}
