
#include "UI_Converter.h"
#include "UI_ButtonColored.h"

UI::UI_ButtonColored::UI_ButtonColored(const std::string& pLabel, const Color& pColor, const glm::vec2& pSize, bool pEnableAlpha) : 
	mLabel(pLabel), mColor(pColor), mSize(pSize), mEnableAlpha(pEnableAlpha)
{
}

void UI::UI_ButtonColored::_Draw_Impl()
{
	ImVec4 imColor = UI_Converter::ToImVec4(mColor);

	if (ImGui::ColorButton((mLabel + mWidgetID).c_str(), imColor, !mEnableAlpha ? ImGuiColorEditFlags_NoAlpha : 0, UI_Converter::ToImVec2(mSize)))
	{
		mClickedEvent.Invoke();
	}

	mColor = UI_Converter::ToColor(imColor);
}
