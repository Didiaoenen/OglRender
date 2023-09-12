
#include "UI_Converter.h"
#include "UI_ButtonImage.h"

#include "imgui_internal.h"

UI::UI_ButtonImage::UI_ButtonImage(uint32_t pTextureID, const glm::vec2& pSize) : 
	mTextureID{ pTextureID }, mSize(pSize)
{
}

void UI::UI_ButtonImage::_Draw_Impl()
{
	ImVec4 bg = UI_Converter::ToImVec4(mBackground);
	ImVec4 tn = UI_Converter::ToImVec4(mTint);

	if (ImGui::ImageButton(mTextureID.mRaw, UI_Converter::ToImVec2(mSize), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), -1, bg, tn, mDisabled ? ImGuiButtonFlags_Disabled : 0))
	{
		mClickedEvent.Invoke();
	}
}
