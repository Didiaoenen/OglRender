
#include <stdexcept>

#include "Window_Device.h"

Tools::Tools_Event<Window::EDeviceError, std::string> Window::Window_Device::mErrorEvent;

Window::Window_Device::Window_Device(const Window_DeviceSettings& pDeviceSettings)
{
	BindErrorCallback();

	int initializationCode = glfwInit();

	if (initializationCode == GLFW_FALSE)
	{
		throw std::runtime_error("Failed to Init GLFW");
		glfwTerminate();
	}
	else
	{
		CreateCursors();

		if (pDeviceSettings.mDebugProfile)
		{
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, pDeviceSettings.mContextMajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, pDeviceSettings.mContextMinorVersion);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, pDeviceSettings.mSamples);

		mIsAlive = true;
	}
}

Window::Window_Device::~Window_Device()
{
	if (mIsAlive)
	{
		DestroyCursors();
		glfwTerminate();
	}
}

std::pair<int16_t, int16_t> Window::Window_Device::GetMonitorSize() const
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	return std::pair<int16_t, int16_t>(static_cast<int16_t>(mode->width), static_cast<int16_t>(mode->height));
}

GLFWcursor* Window::Window_Device::GetCursorInstance(ECursorShape pCursorShape) const
{
	return mCursors.at(pCursorShape);
}

bool Window::Window_Device::HasVsync() const
{
	return mVsync;
}

void Window::Window_Device::SetVsync(bool pValue)
{
	glfwSwapInterval(pValue ? 1 : 0);
	mVsync = pValue;
}

void Window::Window_Device::PollEvents() const
{
	glfwPollEvents();
}

float Window::Window_Device::GetElapsedTime() const
{
	return static_cast<float>(glfwGetTime());
}

void Window::Window_Device::BindErrorCallback()
{
	auto errorCallback = [](int pCode, const char* pDescription)
		{
			mErrorEvent.Invoke(static_cast<EDeviceError>(pCode), pDescription);
		};

	glfwSetErrorCallback(errorCallback);
}

void Window::Window_Device::CreateCursors()
{
	mCursors[ECursorShape::ARROW] = glfwCreateStandardCursor(static_cast<int>(ECursorShape::ARROW));
	mCursors[ECursorShape::IBEAM] = glfwCreateStandardCursor(static_cast<int>(ECursorShape::IBEAM));
	mCursors[ECursorShape::CROSSHAIR] = glfwCreateStandardCursor(static_cast<int>(ECursorShape::CROSSHAIR));
	mCursors[ECursorShape::HAND] = glfwCreateStandardCursor(static_cast<int>(ECursorShape::HAND));
	mCursors[ECursorShape::HRESIZE] = glfwCreateStandardCursor(static_cast<int>(ECursorShape::HRESIZE));
	mCursors[ECursorShape::VRESIZE] = glfwCreateStandardCursor(static_cast<int>(ECursorShape::VRESIZE));
}

void Window::Window_Device::DestroyCursors()
{
	glfwDestroyCursor(mCursors[ECursorShape::ARROW]);
	glfwDestroyCursor(mCursors[ECursorShape::IBEAM]);
	glfwDestroyCursor(mCursors[ECursorShape::CROSSHAIR]);
	glfwDestroyCursor(mCursors[ECursorShape::HAND]);
	glfwDestroyCursor(mCursors[ECursorShape::HRESIZE]);
	glfwDestroyCursor(mCursors[ECursorShape::VRESIZE]);
}
