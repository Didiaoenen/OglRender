#pragma once

#include <string>
#include <stdint.h>

#include "Render_DriverSettings.h"

namespace Render 
{
	class Render_Driver
	{
	public:
		Render_Driver(const Render_DriverSettings& pDriverSettings);

		~Render_Driver() = default;

		bool IsActive() const;

	private:
		void InitGlew();
		static void __stdcall GLDebugMessageCallback(uint32_t pSource, uint32_t pType, uint32_t pId, uint32_t pSeverity, int32_t pLength, const char* pMessage, const void* pUserParam);

	private:
		bool mIsActive{ false };
	};
}

