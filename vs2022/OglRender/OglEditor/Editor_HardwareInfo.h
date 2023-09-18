#pragma once

#include <OglUI/UI_PanelWindow.h>
#include <OglUI/UI_PlotLines.h>
#include <OglUI/UI_PlotHistogram.h>

namespace Editor
{
	class Editor_HardwareInfo;

	class Editor_HardwareInfo : public UI::UI_PanelWindow
	{
	public:
		Editor_HardwareInfo
		(
			const std::string& pTitle,
			bool pOpened,
			const UI::UI_PanelWindowSettings& pWindowSettings,
			float p_logFrequency,
			size_t p_maxElements
		);

		~Editor_HardwareInfo();

		void Update(float pDeltaTime);

	private:
		float p_updateTimer = 0.f;
		float m_logFrequency;
		size_t m_maxElements;
		UI::UI_APlot* m_cpuUsage;
		UI::UI_APlot* m_gpuUsage;
		UI::UI_APlot* m_ramUsage;
	};
}

