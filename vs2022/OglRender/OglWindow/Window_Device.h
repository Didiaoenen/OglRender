#pragma once

#include <unordered_map>

#include <GLFW/glfw3.h>

#include <OglTools/Tools_Event.h>

#include "Window_EDeviceError.h"
#include "Window_ECursorShape.h"
#include "Window_DeviceSettings.h"

namespace Window
{
	class Window_Device
	{
	public:
		static Tools::Tools_Event<EDeviceError, std::string> mErrorEvent;

		Window_Device(const Window_DeviceSettings& pDeviceSettings);

		~Window_Device();

		std::pair<int16_t, int16_t> GetMonitorSize() const;

		GLFWcursor* GetCursorInstance(ECursorShape pCursorShape) const;

		bool HasVsync() const;

		void SetVsync(bool pValue);

		void PollEvents() const;

		float GetElapsedTime() const;

	private:
		void BindErrorCallback();
		void CreateCursors();
		void DestroyCursors();

	private:
		bool mVsync{ true };
		bool mIsAlive{ false };
		std::unordered_map<ECursorShape, GLFWcursor*> mCursors;
	};
}

