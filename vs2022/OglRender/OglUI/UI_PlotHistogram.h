#pragma once

#include <glm/glm.hpp>

#include "UI_APlot.h"

namespace UI
{
	class UI_PlotHistogram : public UI_APlot
	{
	public:
		UI_PlotHistogram
		(
			const std::vector<float>& pData = std::vector<float>(),
			float pMinScale = std::numeric_limits<float>::min(),
			float pMaxScale = std::numeric_limits<float>::max(),
			const glm::vec2& pSize = { 0.0f, 0.0f },
			const std::string& pOverlay = "",
			const std::string& pLabel = "",
			int pForceHover = -1
		);

	protected:
		void _Draw_Impl() override;
	};
}

