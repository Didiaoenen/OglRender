#include "UI_Converter.h"
#include "UI_ButtonSmall.h"

UI::UI_ButtonSmall::UI_ButtonSmall(const std::string& pLabel)
	: mLabel(pLabel)
{
	auto& style = ImGui::GetStyle();

	mIdleBackgroundColor = UI::UI_Converter::ToColor(style.Colors[ImGuiCol_Button]);
	mHoveredBackgroundColor = UI::UI_Converter::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
	mClickedBackgroundColor = UI::UI_Converter::ToColor(style.Colors[ImGuiCol_ButtonActive]);
	mTextColor = UI::UI_Converter::ToColor(style.Colors[ImGuiCol_Text]);
}

void UI::UI_ButtonSmall::_Draw_Impl()
{
	auto& style = ImGui::GetStyle();

	auto defaultIdleColor = style.Colors[ImGuiCol_Button];
	auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
	auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
	auto defaultTextColor = style.Colors[ImGuiCol_Text];

	if (ImGui::SmallButton((mLabel + mWidgetID).c_str()))
	{
		ClickEvent.Invoke();
	}

	style.Colors[ImGuiCol_Button] = UI::UI_Converter::ToImVec4(mIdleBackgroundColor);
	style.Colors[ImGuiCol_ButtonHovered] = UI::UI_Converter::ToImVec4(mHoveredBackgroundColor);
	style.Colors[ImGuiCol_ButtonActive] = UI::UI_Converter::ToImVec4(mClickedBackgroundColor);
	style.Colors[ImGuiCol_Text] = UI::UI_Converter::ToImVec4(mTextColor);
}
