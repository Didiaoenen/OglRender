#pragma once

#include <unordered_map>

#include <OglUI/UI_APanel.h>
#include <OglUI/UI_Canvas.h>
#include <OglUI/UI_PanelWindow.h>

#include "Editor_MenuBar.h"

namespace Editor
{
	class Editor_PanelsManager
	{
	public:
		Editor_PanelsManager(UI::UI_Canvas& p_canvas);

		template<typename T, typename... Args>
		void CreatePanel(const std::string& pId, Args&&... pArgs)
		{
			if constexpr (std::is_base_of<UI::UI_PanelWindow, T>::value)
			{
				m_panels.emplace(pId, std::make_unique<T>(pId, std::forward<Args>(pArgs)...));
				T& instance = *static_cast<T*>(m_panels.at(pId).get());
				GetPanelAs<Editor_MenuBar>("Menu Bar").RegisterPanel(instance.mName, instance);
			}
			else
			{
				m_panels.emplace(pId, std::make_unique<T>(std::forward<Args>(pArgs)...));
			}

			mCanvas.AddPanel(*m_panels.at(pId));
		}

		template<typename T>
		T& GetPanelAs(const std::string& pId)
		{
			return *static_cast<T*>(m_panels[pId].get());
		}

	private:
		std::unordered_map<std::string, std::unique_ptr<UI::UI_APanel>> m_panels;
		UI::UI_Canvas& mCanvas;
	};
}

