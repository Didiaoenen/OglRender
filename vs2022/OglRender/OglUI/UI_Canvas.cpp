#include "UI_Canvas.h"

void UI::UI_Canvas::AddPanel(UI_APanel& pPanel)
{
	mPanels.push_back(std::ref(pPanel));
}

void UI::UI_Canvas::RemovePanel(UI_APanel& pPanel)
{
	mPanels.erase(std::remove_if(mPanels.begin(), mPanels.end(), [&pPanel](std::reference_wrapper<UI_APanel>& p_item)
		{
			return &pPanel == &p_item.get();
		}));
}

void UI::UI_Canvas::RemoveAllPanels()
{
	mPanels.clear();
}

void UI::UI_Canvas::MakeDockspace(bool pState)
{
	mIsDockspace = pState;
}

bool UI::UI_Canvas::IsDockspace() const
{
	return mIsDockspace;
}

void UI::UI_Canvas::Draw()
{
	if (!mPanels.empty())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (mIsDockspace)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("##dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
			
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);
			
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
			ImGui::SetWindowPos({ 0.f, 0.f });
			ImVec2 displaySize = ImGui::GetIO().DisplaySize;
			ImGui::SetWindowSize({ (float)displaySize.x, (float)displaySize.y });
			ImGui::End();

			ImGui::PopStyleVar(3);
		}

		for (auto& panel : mPanels)
		{
			panel.get().Draw();
		}

		ImGui::Render();
	}
}
