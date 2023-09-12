#pragma once

#include <optional>
#include <functional>

namespace UI
{
	class UI_MenuList;
}

namespace Core
{
	class Core_Actor;
}

namespace Editor
{	
	class Editor_ActorCreationMenu
	{
	public:
		Editor_ActorCreationMenu() = delete;

        static void GenerateActorCreationMenu(UI::UI_MenuList& p_menuList, Core::Core_Actor* pParent = nullptr, std::optional<std::function<void()>> p_onItemClicked = {});
	};
}

