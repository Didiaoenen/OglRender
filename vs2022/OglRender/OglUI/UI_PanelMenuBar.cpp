
#include <OglUI/imgui_internal.h>

#include "UI_PanelMenuBar.h"

void UI::UI_PanelMenuBar::_Draw_Impl()
{

	//if (!mWidgets.empty() && ImGui::BeginMainMenuBar())
	//{
	//	DrawWidgets();

	//	ImGui::EndMainMenuBar();
	//}

	if (!mWidgets.empty())
	{
		ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();

		float frameHeight = ImGui::GetFrameHeight();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

		ImGui::SetCurrentViewport(NULL, viewport);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		{
			if (ImGui::BeginViewportSideBar("##MainMenuBar", viewport, ImGuiDir_Up, frameHeight, flags))
			{
				if (ImGui::BeginMenuBar())
				{
					DrawWidgets();

					ImGui::EndMenuBar();
				}

				ImGui::End();
			}

			ImGui::PopStyleVar();
		}
	}
}
