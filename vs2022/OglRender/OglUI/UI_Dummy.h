#pragma once

#include <glm/glm.hpp>

#include "UI_AWidget.h"

namespace UI
{
	class UI_Dummy : public UI_AWidget
	{
	public:
		UI_Dummy(const glm::vec2& pSize = { 0.0f, 0.0f });

	protected:
		void _Draw_Impl() override;

	public:
		glm::vec2 mSize;
	};
}

