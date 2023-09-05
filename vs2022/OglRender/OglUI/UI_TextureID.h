#pragma once

#include <stdint.h>

namespace UI
{
	union TextureID
	{
		uint32_t id;
		void* raw;
	};
}