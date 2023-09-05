#include "UI_Converter.h"

ImVec4 UI::UI_Converter::ToImVec4(const Color& pValue)
{
	return ImVec4(pValue.r, pValue.g, pValue.b, pValue.a);
}

UI::Color UI::UI_Converter::ToColor(const ImVec4& pValue)
{
	return UI::Color(pValue.x, pValue.y, pValue.z, pValue.w);
}

ImVec2 UI::UI_Converter::ToImVec2(const glm::vec2& pValue)
{
	return ImVec2(pValue.x, pValue.y);
}

glm::vec2 UI::UI_Converter::ToGlmVec2(const ImVec2& pValue)
{
	return glm::vec2(pValue.x, pValue.y);
}
