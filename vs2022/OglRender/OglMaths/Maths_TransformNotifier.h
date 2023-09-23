#pragma once

#include <functional>

namespace Maths
{
	class Maths_TransformNotifier
	{
	public:
		enum class ENotification
		{
			TRANSFORM_CHANGED,
			TRANSFORM_DESTROYED,
		};

		using NotificationHandler = std::function<void(ENotification)>;

		using NotificationHandlerID = uint64_t;

		NotificationHandlerID AddNotificationHandler(NotificationHandler pNotificationHandler);

		void NotifyChildren(ENotification pNotification);

		bool RemoveNotificationHandler(const NotificationHandlerID& pNotificationHandlerID);

	private:
		std::unordered_map<NotificationHandlerID, NotificationHandler> mNotificationHandlers;
		NotificationHandlerID mAvailableHandlerID{ 0 };
	};
}

