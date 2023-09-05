#pragma once

#include <glm/glm.hpp>

#include "imgui.h"
#include "UI_Color.h"

namespace UI
{
class UI_Converter
{
public:
	UI_Converter() = default;

	static ImVec4 ToImVec4(const Color& pValue);

	static Color ToColor(const ImVec4& pValue);

	static ImVec2 ToImVec2(const glm::vec2& pValue);

	static glm::vec2 ToGlmVec2(const ImVec2& pValue);
};
}

