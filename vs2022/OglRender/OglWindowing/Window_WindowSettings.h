#pragma once

#include <string>

#include "Window_ECursorShape.h"
#include "Window_ECursorMode.h"

namespace Window
{
	struct WindowSettings
	{
		static const int32_t DontCare = -1;

		std::string title;

		uint16_t width;

		uint16_t height;

		int16_t minimumWidth = DontCare;

		int16_t minimumHeight = DontCare;

		int16_t maximumWidth = DontCare;

		int16_t maximumHeight = DontCare;

		bool fullscreen = false;

		bool decorated = true;

		bool resizable = true;

		bool focused = true;

		bool maximized = false;

		bool floating = false;

		bool visible = true;

		bool autoIconify = true;

		int32_t refreshRate = WindowSettings::DontCare;

		ECursorMode cursorMode = ECursorMode::NORMAL;

		ECursorShape cursorShape = ECursorShape::ARROW;

		uint32_t samples = 4;
	};
}