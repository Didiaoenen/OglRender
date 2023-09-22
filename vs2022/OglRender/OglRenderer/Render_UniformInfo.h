#pragma once

#include <any>
#include <string>

#include "Render_UniformType.h"

namespace Render
{
	struct Render_UniformInfo
	{
		EUniformType		type;
		std::string		name;
		uint32_t		location;
		std::any		defaultValue;
	};
}