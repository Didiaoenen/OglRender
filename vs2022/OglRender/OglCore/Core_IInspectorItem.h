#pragma once

#include <OglUI/UI_WidgetContainer.h>

#include "Core_ISerializable.h"
#include "Core_GUIDrawer.h"

namespace Core
{
	class Core_IInspectorItem : public Core_ISerializable
	{
	public:
		virtual void OnInspector(UI::UI_WidgetContainer& p_root) = 0;
	};
}