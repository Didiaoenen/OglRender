#pragma once

#include <string>
#include <stdint.h>

#include "Render_DriverSettings.h"

namespace Render 
{
	class Render_Driver
	{
	public:
		Render_Driver(const Render_DriverSettings& p_driverSettings);

		~Render_Driver() = default;

		bool IsActive() const;

	private:
		void InitGlew();
		static void __stdcall GLDebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam);

	private:
		bool m_isActive;
	};
}

