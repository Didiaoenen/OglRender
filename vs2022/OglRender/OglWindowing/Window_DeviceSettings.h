#pragma once

#include <string>

namespace Window
{
	struct Window_DeviceSettings
	{
		bool mDebugProfile = false;

		bool mForwardCompatibility = false;

		uint8_t mContextMajorVersion = 3;

		uint8_t mContextMinorVersion = 2;

		uint8_t mSamples = 4;

	};
}

