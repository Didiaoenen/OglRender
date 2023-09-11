#include <iostream>

#include <stb_image.h>

#include "Window_Window.h"

std::unordered_map<GLFWwindow*, Window::Window_Window*> Window::Window_Window::__WINDOWS_MAP;

Window::Window_Window::Window_Window(const Window_Device& pDevice, const Window_WindowSettings& pWindowSettings)
	: mDevice(pDevice), mTitle(pWindowSettings.title), mSize{ pWindowSettings.width, pWindowSettings.height },
	mMinimumSize{ pWindowSettings.minimumWidth, pWindowSettings.minimumHeight },
	mMaximumSize{ pWindowSettings.maximumWidth, pWindowSettings.maximumHeight },
	mFullscreen(pWindowSettings.fullscreen),
	mRefreshRate(pWindowSettings.refreshRate),
	mCursorMode(pWindowSettings.cursorMode),
	mCursorShape(pWindowSettings.cursorShape)
{
	CreateGlfwWindow(pWindowSettings);

	SetCursorMode(pWindowSettings.cursorMode);
	SetCursorShape(pWindowSettings.cursorShape);

	BindKeyCallback();
	BindMouseCallback();
	BindIconifyCallback();
	BindCloseCallback();
	BindResizeCallback();
	BindCursorMoveCallback();
	BindFramebufferResizeCallback();
	BindMoveCallback();
	BindFocusCallback();

	mResizeEvent.AddListener(std::bind(&Window_Window::OnResize, this, std::placeholders::_1, std::placeholders::_2));
	mMoveEvent.AddListener(std::bind(&Window_Window::OnMove, this, std::placeholders::_1, std::placeholders::_2));
}

Window::Window_Window::~Window_Window()
{
	glfwDestroyWindow(mGlfwWindow);
}

void Window::Window_Window::SetIcon(const std::string& pFilePath)
{
	GLFWimage images[1];
	images[0].pixels = stbi_load(pFilePath.c_str(), &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(mGlfwWindow, 1, images);
}

void Window::Window_Window::SetIconFromMemory(uint8_t* pData, uint32_t pWidth, uint32_t pHeight)
{
	GLFWimage images[1];
	images[0].pixels = pData;
	images[0].height = pWidth;
	images[0].width = pHeight;
	glfwSetWindowIcon(mGlfwWindow, 1, images);
}

Window::Window_Window* Window::Window_Window::FindInstance(GLFWwindow* pGlfwWindow)
{
	return __WINDOWS_MAP.find(pGlfwWindow) != __WINDOWS_MAP.end() ? __WINDOWS_MAP[pGlfwWindow] : nullptr;
}

void Window::Window_Window::SetSize(uint16_t pWidth, uint16_t pHeight)
{
	glfwSetWindowSize(mGlfwWindow, static_cast<int>(pWidth), static_cast<int>(pHeight));
}

void Window::Window_Window::SetMinimumSize(int16_t pMinimumWidth, int16_t pMinimumHeight)
{
	mMinimumSize.first = pMinimumWidth;
	mMinimumSize.second = pMinimumHeight;

	UpdateSizeLimit();
}

void Window::Window_Window::SetMaximumSize(int16_t pMaximumWidth, int16_t pMaximumHeight)
{
	mMaximumSize.first = pMaximumWidth;
	mMaximumSize.second = pMaximumHeight;

	UpdateSizeLimit();
}

void Window::Window_Window::SetPosition(int16_t pX, int16_t pY)
{
	glfwSetWindowPos(mGlfwWindow, static_cast<int>(pX), static_cast<int>(pY));
}

void Window::Window_Window::Minimize() const
{
	glfwIconifyWindow(mGlfwWindow);
}

void Window::Window_Window::Maximize() const
{
	glfwMaximizeWindow(mGlfwWindow);
}

void Window::Window_Window::Restore() const
{
	glfwRestoreWindow(mGlfwWindow);
}

void Window::Window_Window::Hide() const
{
	glfwHideWindow(mGlfwWindow);
}

void Window::Window_Window::Show() const
{
	glfwShowWindow(mGlfwWindow);
}

void Window::Window_Window::Focus() const
{
	glfwFocusWindow(mGlfwWindow);
}

void Window::Window_Window::SetShouldClose(bool pValue) const
{
	glfwSetWindowShouldClose(mGlfwWindow, pValue);
}

bool Window::Window_Window::ShouldClose() const
{
	return glfwWindowShouldClose(mGlfwWindow);
}

void Window::Window_Window::SetFullscreen(bool pValue)
{
	if (pValue)
	{
		mFullscreen = true;
	}

	glfwSetWindowMonitor
	(
		mGlfwWindow,
		pValue ? glfwGetPrimaryMonitor() : nullptr,
		static_cast<int>(mPosition.first),
		static_cast<int>(mPosition.second),
		static_cast<int>(mSize.first),
		static_cast<int>(mSize.second),
		mRefreshRate
	);

	if (!pValue)
	{
		mFullscreen = false;
	}
}

void Window::Window_Window::ToggleFullscreen()
{
	SetFullscreen(!mFullscreen);
}

bool Window::Window_Window::IsFullscreen() const
{
	return mFullscreen;
}

bool Window::Window_Window::IsHidden() const
{
	return glfwGetWindowAttrib(mGlfwWindow, GLFW_VISIBLE) == GLFW_FALSE;
}

bool Window::Window_Window::IsVisible() const
{
	return glfwGetWindowAttrib(mGlfwWindow, GLFW_VISIBLE) == GLFW_TRUE;
}

bool Window::Window_Window::IsMaximized() const
{
	return glfwGetWindowAttrib(mGlfwWindow, GLFW_MAXIMIZED) == GLFW_TRUE;
}

bool Window::Window_Window::IsMinimized() const
{
	return glfwGetWindowAttrib(mGlfwWindow, GLFW_MAXIMIZED) == GLFW_FALSE;
}

bool Window::Window_Window::IsFocused() const
{
	return glfwGetWindowAttrib(mGlfwWindow, GLFW_FOCUSED) == GLFW_TRUE;
}

bool Window::Window_Window::IsResizable() const
{
	return glfwGetWindowAttrib(mGlfwWindow, GLFW_RESIZABLE) == GLFW_TRUE;
}

bool Window::Window_Window::IsDecorated() const
{
	return glfwGetWindowAttrib(mGlfwWindow, GLFW_DECORATED) == GLFW_TRUE;
}

void Window::Window_Window::MakeCurrentContext() const
{
	glfwMakeContextCurrent(mGlfwWindow);
}

void Window::Window_Window::SwapBuffers() const
{
	glfwSwapBuffers(mGlfwWindow);
}

void Window::Window_Window::SetCursorMode(ECursorMode pCursorMode)
{
	mCursorMode = pCursorMode;
	glfwSetInputMode(mGlfwWindow, GLFW_CURSOR, static_cast<int>(pCursorMode));
}

void Window::Window_Window::SetCursorShape(ECursorShape pCursorShape)
{
	mCursorShape = pCursorShape;
	glfwSetCursor(mGlfwWindow, mDevice.GetCursorInstance(pCursorShape));
}

void Window::Window_Window::SetCursorPosition(int16_t pX, int16_t pY)
{
	glfwSetCursorPos(mGlfwWindow, static_cast<double>(pX), static_cast<double>(pY));
}

void Window::Window_Window::SetTitle(const std::string& pTitle)
{
	mTitle = pTitle;
	glfwSetWindowTitle(mGlfwWindow, pTitle.c_str());
}

void Window::Window_Window::SetRefreshRate(int32_t pRefreshRate)
{
	mRefreshRate = pRefreshRate;
}

std::string Window::Window_Window::GetTitle() const
{
	return mTitle;
}

std::pair<uint16_t, uint16_t> Window::Window_Window::GetSize() const
{
	int width, height;
	glfwGetWindowSize(mGlfwWindow, &width, &height);
	return std::make_pair(static_cast<uint16_t>(width), static_cast<uint16_t>(height));
}

std::pair<int16_t, int16_t> Window::Window_Window::GetMinimumSize() const
{
	return mMinimumSize;
}

std::pair<int16_t, int16_t> Window::Window_Window::GetMaximumSize() const
{
	return mMaximumSize;
}

std::pair<int16_t, int16_t> Window::Window_Window::GetPosition() const
{
	int x, y;
	glfwGetWindowPos(mGlfwWindow, &x, &y);
	return std::make_pair(static_cast<int16_t>(x), static_cast<int16_t>(y));
}

std::pair<uint16_t, uint16_t> Window::Window_Window::GetFramebufferSize() const
{
	int width, height;
	glfwGetFramebufferSize(mGlfwWindow, &width, &height);
	return std::make_pair(static_cast<uint16_t>(width), static_cast<uint16_t>(height));
}

Window::ECursorMode Window::Window_Window::GetCursorMode() const
{
	return mCursorMode;
}

Window::ECursorShape Window::Window_Window::GetCursorShape() const
{
	return mCursorShape;
}

int32_t Window::Window_Window::GetRefreshRate() const
{
	return mRefreshRate;
}

GLFWwindow* Window::Window_Window::GetGlfwWindow() const
{
	return mGlfwWindow;
}

void Window::Window_Window::CreateGlfwWindow(const Window_WindowSettings& pWindowSettings)
{
	GLFWmonitor* selectedMonitor = nullptr;

	if (mFullscreen)
	{
		selectedMonitor = glfwGetPrimaryMonitor();
	}

	glfwWindowHint(GLFW_RESIZABLE, pWindowSettings.resizable);
	glfwWindowHint(GLFW_DECORATED, pWindowSettings.decorated);
	glfwWindowHint(GLFW_FOCUSED, pWindowSettings.focused);
	glfwWindowHint(GLFW_MAXIMIZED, pWindowSettings.maximized);
	glfwWindowHint(GLFW_FLOATING, pWindowSettings.floating);
	glfwWindowHint(GLFW_VISIBLE, pWindowSettings.visible);
	glfwWindowHint(GLFW_AUTO_ICONIFY, pWindowSettings.autoIconify);
	glfwWindowHint(GLFW_REFRESH_RATE, pWindowSettings.refreshRate);
	glfwWindowHint(GLFW_SAMPLES, pWindowSettings.samples);

	mGlfwWindow = glfwCreateWindow(static_cast<int>(mSize.first), static_cast<int>(mSize.second), mTitle.c_str(), selectedMonitor, nullptr);

	if (!mGlfwWindow)
	{
		throw std::runtime_error("Failed to create GLFW window");
	}
	else
	{
		UpdateSizeLimit();

		auto [x, y] = GetPosition();
		mPosition.first = x;
		mPosition.second = y;

		__WINDOWS_MAP[mGlfwWindow] = this;
	}
}

void Window::Window_Window::BindKeyCallback() const
{
	auto keyCallback = [](GLFWwindow* pWindow, int pKey, int pScancode, int pAction, int pMods)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				if (pAction == GLFW_PRESS)
				{
					windowInstance->mKeyPressedEvent.Invoke(pKey);
				}

				if (pAction == GLFW_RELEASE)
				{
					windowInstance->mKeyReleasedEvent.Invoke(pKey);
				}
			}
		};

	glfwSetKeyCallback(mGlfwWindow, keyCallback);
}

void Window::Window_Window::BindMouseCallback() const
{
	auto mouseCallback = [](GLFWwindow* pWindow, int pButton, int pAction, int pMods)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				if (pAction == GLFW_PRESS)
				{
					windowInstance->mMouseButtonPressedEvent.Invoke(pButton);
				}

				if (pAction == GLFW_RELEASE)
				{
					windowInstance->mMouseButtonReleasedEvent.Invoke(pButton);
				}
			}
		};

	glfwSetMouseButtonCallback(mGlfwWindow, mouseCallback);
}

void Window::Window_Window::BindResizeCallback() const
{
	auto resizeCallback = [](GLFWwindow* pWindow, int pWidth, int pHeight)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				windowInstance->mResizeEvent.Invoke(static_cast<uint16_t>(pWidth), static_cast<uint16_t>(pHeight));
			}
		};

	glfwSetWindowSizeCallback(mGlfwWindow, resizeCallback);
}

void Window::Window_Window::BindFramebufferResizeCallback() const
{
	auto framebufferResizeCallback = [](GLFWwindow* pWindow, int pWidth, int pHeight)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				windowInstance->mFramebufferResizeEvent.Invoke(static_cast<uint16_t>(pWidth), static_cast<uint16_t>(pHeight));
			}
		};

	glfwSetFramebufferSizeCallback(mGlfwWindow, framebufferResizeCallback);
}

void Window::Window_Window::BindCursorMoveCallback() const
{
	auto cursorMoveCallback = [](GLFWwindow* pWindow, double pX, double pY)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				windowInstance->mCursorMoveEvent.Invoke(static_cast<int16_t>(pX), static_cast<int16_t>(pY));
			}
		};

	glfwSetCursorPosCallback(mGlfwWindow, cursorMoveCallback);
}

void Window::Window_Window::BindMoveCallback() const
{
	auto moveCallback = [](GLFWwindow* pWindow, int pX, int pY)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				windowInstance->mMoveEvent.Invoke(static_cast<int16_t>(pX), static_cast<int16_t>(pY));
			}
		};

	glfwSetWindowPosCallback(mGlfwWindow, moveCallback);
}

void Window::Window_Window::BindIconifyCallback() const
{
	auto iconifyCallback = [](GLFWwindow* pWindow, int pIconified)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				if (pIconified == GLFW_TRUE)
				{
					windowInstance->mMinimizeEvent.Invoke();
				}

				if (pIconified == GLFW_FALSE)
				{
					windowInstance->mMaximizeEvent.Invoke();
				}
			}
		};

	glfwSetWindowIconifyCallback(mGlfwWindow, iconifyCallback);
}

void Window::Window_Window::BindFocusCallback() const
{
	auto focusCallback = [](GLFWwindow* pWindow, int pFocused)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				if (pFocused == GLFW_TRUE)
				{
					windowInstance->mGainFocusEvent.Invoke();
				}

				if (pFocused == GLFW_FALSE)
				{
					windowInstance->mLostFocusEvent.Invoke();
				}
			}
		};

	glfwSetWindowFocusCallback(mGlfwWindow, focusCallback);
}

void Window::Window_Window::BindCloseCallback() const
{
	auto closeCallback = [](GLFWwindow* pWindow)
		{
			Window_Window* windowInstance = FindInstance(pWindow);

			if (windowInstance)
			{
				windowInstance->mCloseEvent.Invoke();
			}
		};

	glfwSetWindowCloseCallback(mGlfwWindow, closeCallback);
}

void Window::Window_Window::OnResize(uint16_t pWidth, uint16_t pHeight)
{
	mSize.first = pWidth;
	mSize.second = pHeight;
}

void Window::Window_Window::OnMove(int16_t pX, int16_t pY)
{
	if (!mFullscreen)
	{
		mPosition.first = pX;
		mPosition.second = pY;
	}
}

void Window::Window_Window::UpdateSizeLimit() const
{
	glfwSetWindowSizeLimits
	(
		mGlfwWindow,
		static_cast<int>(mMinimumSize.first),
		static_cast<int>(mMinimumSize.second),
		static_cast<int>(mMaximumSize.first),
		static_cast<int>(mMaximumSize.second)
	);
}
