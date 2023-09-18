#include "Editor_HardwareInfo.h"

Editor::Editor_HardwareInfo::Editor_HardwareInfo(const std::string& pTitle, bool pOpened, const UI::UI_PanelWindowSettings& pWindowSettings, float p_logFrequency, size_t p_maxElements) :
	UI::UI_PanelWindow(pTitle, pOpened, pWindowSettings),
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
