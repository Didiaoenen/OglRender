#pragma once

#include <OglTools/Tools_Event.h>

#include "UI_DataWidget.h"
#include "UI_Color.h"

namespace UI
{
	class UI_ColorEdit : public UI_DataWidget<Color>
	{
	public:
		UI_ColorEdit(bool pEnableAlpha = false, const Color& pDefaultColor = {});

	protected:
		void _Draw_Impl() override;

	public:
		bool mEnableAlpha;
		Color mColor;
		Tools::Tools_Event<Color&> mColorChangedEvent;
	};
}

