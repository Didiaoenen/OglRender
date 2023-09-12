#include "Core_Actor.h"
#include "Core_AComponent.h"

Core::Core_AComponent::Core_AComponent(Core_Actor& pOwner) : 
	mOwner(pOwner)
{
}

Core::Core_AComponent::~Core_AComponent()
{
	if (mOwner.IsActive())
	{
		OnDisable();
		OnDestroy();
	}
}
