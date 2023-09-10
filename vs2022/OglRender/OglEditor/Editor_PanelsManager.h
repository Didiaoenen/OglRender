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
		void CreatePanel(const std::string& p_id, Args&&... p_args)
		{
			if constexpr (std::is_base_of<UI::UI_PanelWindow, T>::value)
			{
				m_panels.emplace(p_id, std::make_unique<T>(p_id, std::forward<Args>(p_args)...));
				T& instance = *static_cast<T*>(m_panels.at(p_id).get());
				GetPanelAs<Editor_MenuBar>("Menu Bar").RegisterPanel(instance.name, instance);
			}
			else
			{
				m_panels.emplace(p_id, std::make_unique<T>(std::forward<Args>(p_args)...));
			}

			m_canvas.AddPanel(*m_panels.at(p_id));
		}

		template<typename T>
		T& GetPanelAs(const std::string& p_id)
		{
			return *static_cast<T*>(m_panels[p_id].get());
		}

	private:
		std::unordered_map<std::string, std::unique_ptr<UI::UI_APanel>> m_panels;
		UI::UI_Canvas& m_canvas;
	};
}

