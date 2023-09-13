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

		void RegisterProject(const std::string& pPath);

	private:
		std::unique_ptr<Window::Window_Device>		mDevice;
		std::unique_ptr<Window::Window_Window>		mWindow;
		std::unique_ptr<Render::Render_Driver>		mDriver;
		std::unique_ptr<Render::Render_Renderer>	mRenderer;
		std::unique_ptr<UI::UI_UIManager>			mUIManager;

		UI::UI_Canvas m_canvas;
		std::unique_ptr<UI::UI_PanelWindow>			mMainPanel;

		std::string mProjectPath = "";
		std::string mProjectName = "";
		bool mReadyToGo = false;
	};
}

