
#include <OglUI/UI_Separator.h>

#include "Editor_Profiler.h"

Editor::Editor_Profiler::Editor_Profiler(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings, float p_frequency) :
	UI::UI_PanelWindow(p_title, p_opened, p_windowSettings), m_frequency(p_frequency)
{
	SetIcon(ICON_MDI_GAUGE " ");
}

void Editor::Editor_Profiler::Update(float pDeltaTime)
{
}

void Editor::Editor_Profiler::Enable(bool pValue, bool p_disableLog)
{
}

UI::Color Editor::Editor_Profiler::CalculateActionColor(double p_percentage) const
{
	return UI::Color();
}
