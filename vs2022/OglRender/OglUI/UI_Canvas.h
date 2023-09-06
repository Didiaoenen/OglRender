#pragma once

#include <algorithm>

#include "OglTools/Tools_Event.h"
#include "UI_IDrawable.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace UI
{

	class UI_APanel;

	class UI_Canvas : public UI_IDrawable
	{
	public:
		void AddPanel(UI_APanel& p_panel);

		void RemovePanel(UI_APanel& p_panel);

		void RemoveAllPanels();

		void MakeDockspace(bool p_state);

		bool IsDockspace() const;

		void Draw() override;

	private:
		std::vector<std::reference_wrapper<UI_APanel>> mPanels;
		bool mIsDockspace = false;
	};
}