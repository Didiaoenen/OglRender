#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include "Window_Device.h"
#include "Window_WindowSettings.h"
#include "Window_ECursorMode.h"
#include "Window_ECursorMode.h"

namespace Window
{
	class Window_Window
	{
	public:
		Tools::Tools_Event<int> mKeyPressedEvent;
		Tools::Tools_Event<int> mKeyReleasedEvent;
		Tools::Tools_Event<int> mMouseButtonPressedEvent;
		Tools::Tools_Event<int> mMouseButtonReleasedEvent;

		Tools::Tools_Event<uint16_t, uint16_t> mResizeEvent;
		Tools::Tools_Event<uint16_t, uint16_t> mFramebufferResizeEvent;
		Tools::Tools_Event<int16_t, int16_t> mMoveEvent;
		Tools::Tools_Event<int16_t, int16_t> mCursorMoveEvent;
		Tools::Tools_Event<> mMinimizeEvent;
		Tools::Tools_Event<> mMaximizeEvent;
		Tools::Tools_Event<> mGainFocusEvent;
		Tools::Tools_Event<> mLostFocusEvent;
		Tools::Tools_Event<> mCloseEvent;

		Window_Window(const Window_Device& pDevice, const Window_WindowSettings& pWindowSettings);

		~Window_Window();

		void SetIcon(const std::string& pFilePath);

		void SetIconFromMemory(uint8_t* pData, uint32_t pWidth, uint32_t pHeight);

		static Window_Window* FindInstance(GLFWwindow* pGlfwWindow);

		void SetSize(uint16_t pWidth, uint16_t pHeight);

		void SetMinimumSize(int16_t pMinimumWidth, int16_t pMinimumHeight);

		void SetMaximumSize(int16_t pMaximumWidth, int16_t pMaximumHeight);

		void SetPosition(int16_t pX, int16_t pY);

		void Minimize() const;

		void Maximize() const;

		void Restore() const;

		void Hide() const;

		void Show() const;

		void Focus() const;

		void SetShouldClose(bool pValue) const;

		bool ShouldClose() const;

		void SetFullscreen(bool pValue);

		void ToggleFullscreen();

		bool IsFullscreen() const;

		bool IsHidden() const;

		bool IsVisible() const;

		bool IsMaximized() const;

		bool IsMinimized() const;

		bool IsFocused() const;

		bool IsResizable() const;

		bool IsDecorated() const;

		void MakeCurrentContext() const;

		void SwapBuffers() const;

		void SetCursorMode(ECursorMode pCursorMode);

		void SetCursorShape(ECursorShape pCursorShape);

		void SetCursorPosition(int16_t pX, int16_t pY);

		void SetTitle(const std::string& pTitle);

		void SetRefreshRate(int32_t pRefreshRate);

		std::string GetTitle() const;

		std::pair<uint16_t, uint16_t> GetSize() const;

		std::pair<int16_t, int16_t> GetMinimumSize() const;

		std::pair<int16_t, int16_t> GetMaximumSize() const;

		std::pair<int16_t, int16_t> GetPosition() const;

		std::pair<uint16_t, uint16_t> GetFramebufferSize() const;

		ECursorMode GetCursorMode() const;

		ECursorShape GetCursorShape() const;

		int32_t GetRefreshRate() const;

		GLFWwindow* GetGlfwWindow() const;

	private:
		void CreateGlfwWindow(const Window_WindowSettings& pWindowSettings);

		void BindKeyCallback() const;
		void BindMouseCallback() const;
		void BindResizeCallback() const;
		void BindFramebufferResizeCallback() const;
		void BindCursorMoveCallback() const;
		void BindMoveCallback() const;
		void BindIconifyCallback() const;
		void BindFocusCallback() const;
		void BindCloseCallback() const;

		void OnResize(uint16_t pWidth, uint16_t pHeight);
		void OnMove(int16_t pX, int16_t pY);

		void UpdateSizeLimit() const;

	private:
		static std::unordered_map<GLFWwindow*, Window_Window*> __WINDOWS_MAP;

		const Window_Device& mDevice;
		GLFWwindow* mGlfwWindow;

		std::string mTitle;
		std::pair<uint16_t, uint16_t> mSize;
		std::pair<int16_t, int16_t> mMinimumSize;
		std::pair<int16_t, int16_t> mMaximumSize;
		std::pair<int16_t, int16_t> mPosition;
		bool mFullscreen;
		int32_t mRefreshRate;
		ECursorMode mCursorMode;
		ECursorShape mCursorShape;
	};
}