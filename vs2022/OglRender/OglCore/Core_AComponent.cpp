#include "Core_AComponent.h"
#include "Core_Actor.h"

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
