
#include "UI_Button.h"
#include "UI_Converter.h"
#include "imgui_internal.h"

UI::UI_Button::UI_Button(const std::string& pLabel, const glm::vec2& pSize, bool pDisabled)
	: mLabel(pLabel), mSize(pSize), mDisabled(pDisabled)
{
	auto& style = ImGui::GetStyle();

	mIdleBackgroundColor = UI::UI_Converter::ToColor(style.Colors[ImGuiCol_Button]);
	mHoveredBackgroundColor = UI::UI_Converter::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
	mClickedBackgroundColor = UI::UI_Converter::ToColor(style.Colors[ImGuiCol_ButtonActive]);
	mTextColor = UI::UI_Converter::ToColor(style.Colors[ImGuiCol_Text]);
}

void UI::UI_Button::_Draw_Impl()
{
	auto& style = ImGui::GetStyle();

	auto defaultIdleColor = style.Colors[ImGuiCol_Button];
	auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
	auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
	auto defaultTextColor = style.Colors[ImGuiCol_Text];

	style.Colors[ImGuiCol_Button] = UI::UI_Converter::ToImVec4(mIdleBackgroundColor);
	style.Colors[ImGuiCol_ButtonHovered] = UI::UI_Converter::ToImVec4(mHoveredBackgroundColor);
	style.Colors[ImGuiCol_ButtonActive] = UI::UI_Converter::ToImVec4(mClickedBackgroundColor);
	style.Colors[ImGuiCol_Text] = UI::UI_Converter::ToImVec4(mTextColor);

	if (ImGui::ButtonEx((mLabel + mWidgetID).c_str(), UI::UI_Converter::ToImVec2(mSize), mDisabled ? ImGuiButtonFlags_Disabled : 0))
	{
		mClickedEvent.Invoke();
	}

	style.Colors[ImGuiCol_Button] = defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
	style.Colors[ImGuiCol_Text] = defaultTextColor;
}
