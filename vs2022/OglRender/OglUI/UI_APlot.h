#pragma once

#include <vector>
#include <limits>
#include <glm/glm.hpp>

#include "UI_AWidget.h"
#include "UI_DataWidget.h"

namespace UI
{
	class UI_APlot : public UI_DataWidget<std::vector<float>>
	{
	public:
		UI_APlot
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
		virtual void _Draw_Impl() override = 0;

	public:
		std::vector<float> mData;
		float mMinScale;
		float mMaxScale;
		glm::vec2 mSize;
		std::string mOverlay;
		std::string mLabel;
		int mForceHover;
	};
}

