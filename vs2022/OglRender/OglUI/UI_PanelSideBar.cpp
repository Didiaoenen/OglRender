
#include "imgui_internal.h"

#include "UI_PanelSideBar.h"

void UI::UI_PanelSideBar::_Draw_Impl()
{
	if (!mWidgets.empty())
	{
		auto* viewport = ImGui::GetMainViewport();
		float frameHeight = ImGui::GetFrameHeight();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 6.0f });
			if (ImGui::BeginViewportSideBar(GetPanelID().c_str(), viewport, ImGuiDir_Up, frameHeight, flags))
			{
				ImGui::PopStyleVar();

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 0));
				if (ImGui::BeginMenuBar())
				{
					ImVec2 maxRegion = ImGui::GetContentRegionMax();

					int buttonCount = mWidgets.size();
					float buttonStartPositionX = maxRegion.x * 0.5f - buttonCount * 0.5f * frameHeight;
					
					ImGui::SetCursorPosX(buttonStartPositionX);
					//ImGui::SetCursorPosY(15);

					DrawWidgets();

					ImGui::EndMenuBar();
				}
				ImGui::PopStyleVar();

				ImGui::End();
			}

			ImGui::PopStyleVar();
		}
	}

}
