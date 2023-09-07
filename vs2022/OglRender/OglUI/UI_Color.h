#pragma once

#include <glm/glm.hpp>

namespace UI
{
	typedef glm::vec4 Color;

	const Color Red { 1.f, 0.f, 0.f, 1.f };
	const Color Green { 0.f, 1.f, 0.f, 1.f };
	const Color Blue { 0.f, 0.f, 1.f, 1.f };
	const Color White { 1.f, 1.f, 1.f, 1.f };
	const Color Black { 0.f, 0.f, 0.f, 1.f };
	const Color Grey { 0.5f, 0.5f, 0.5f, 1.f };
	const Color Yellow { 1.f, 1.f, 0.f, 1.f };
	const Color Cyan { 0.f, 1.f, 1.f, 1.f };
	const Color Magenta { 1.f, 0.f, 1.f, 1.f };
}