#pragma once

#include <OglRenderer/Render_Driver.h>
#include <OglRenderer/Render_Renderer.h>

#include <OglWindow/Window_Device.h>
#include <OglWindow/Window_Window.h>

#include <OglUI/UI_UIManager.h>
#include <OglUI/UI_PanelWindow.h>

namespace Editor
{
	class Editor_ProjectHub
	{
	public:
		Editor_ProjectHub();

		std::tuple<bool, std::string, std::string> Run();

		void SetupContext();

		void RegisterProject(const std::string& p_path);

	private:
		std::unique_ptr<Window::Window_Device>		m_device;
		std::unique_ptr<Window::Window_Window>		m_window;
		std::unique_ptr<Render::Render_Driver>		m_driver;
		std::unique_ptr<Render::Render_Renderer>	m_renderer;
		std::unique_ptr<UI::UI_UIManager>			m_uiManager;

		UI::UI_Canvas m_canvas;
		std::unique_ptr<UI::UI_PanelWindow>			m_mainPanel;

		std::string m_projectPath = "";
		std::string m_projectName = "";
		bool m_readyToGo = false;
	};
}

