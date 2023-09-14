#pragma once

#include <OglUI/UI_MenuItem.h>
#include <OglUI/UI_PanelWindow.h>
#include <OglUI/UI_PanelMenuBar.h>

namespace Editor
{
	class Editor_MenuBar : public UI::UI_PanelMenuBar
	{
		using PanelMap = std::unordered_map<std::string, std::pair<std::reference_wrapper<UI::UI_PanelWindow>, std::reference_wrapper<UI::UI_MenuItem>>>;

	public:
		Editor_MenuBar();

		void HandleShortcuts(float pDeltaTime);

		void RegisterPanel(const std::string& pName, UI::UI_PanelWindow& pPanel);

	private:
		void CreateFileMenu();
		void CreateBuildMenu();
		void CreateWindowMenu();
		void CreateActorsMenu();
		void CreateResourcesMenu();
		void CreateSettingsMenu();
		void CreateLayoutMenu();
		void CreateHelpMenu();

		void UpdateToggleableItems();
		void OpenEveryWindows(bool pState);

	private:
		PanelMap m_panels;

		UI::UI_MenuList* m_windowMenu = nullptr;
	};
}

