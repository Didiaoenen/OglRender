#include "Maths_TransformNotifier.h"

Maths::Maths_TransformNotifier::NotificationHandlerID Maths::Maths_TransformNotifier::AddNotificationHandler(NotificationHandler pNotificationHandler)
{
	NotificationHandlerID handlerID = mAvailableHandlerID++;
	mNotificationHandlers.emplace(handlerID, pNotificationHandler);
	return handlerID;
}

void Maths::Maths_TransformNotifier::NotifyChildren(ENotification pNotification)
{
	if (!mNotificationHandlers.empty())
	{
		for (auto const& [id, handler] : mNotificationHandlers)
		{
			handler(pNotification);
		}
	}
}

bool Maths::Maths_TransformNotifier::RemoveNotificationHandler(const NotificationHandlerID& pNotificationHandlerID)
{
	return mNotificationHandlers.erase(pNotificationHandlerID) != 0;
}
