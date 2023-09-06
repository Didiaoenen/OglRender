#pragma once

#include <glm/glm.hpp>

#include "UI_AWidget.h"

namespace UI
{
	class UI_ProgressBar : public UI_AWidget
	{
	public:
		UI_ProgressBar(float pFraction = 0.0f, const glm::vec2& pSize = { 0.0f, 0.0f }, const std::string& pOverlay = "");

	protected:
		void _Draw_Impl() override;

	public:
		float mFraction;
		glm::vec2 mSize;
		std::string mOverlay;
	};
}

