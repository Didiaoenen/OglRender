#include "Editor_HardwareInfo.h"

Editor::Editor_HardwareInfo::Editor_HardwareInfo(const std::string& p_title, bool p_opened, const UI::UI_PanelWindowSettings& p_windowSettings, float p_logFrequency, size_t p_maxElements) :
	UI::UI_PanelWindow(p_title, p_opened, p_windowSettings),
	m_logFrequency(p_logFrequency),
	m_maxElements(p_maxElements)
{
}

Editor::Editor_HardwareInfo::~Editor_HardwareInfo()
{
}

void Editor::Editor_HardwareInfo::Update(float pDeltaTime)
{
}
