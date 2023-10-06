#pragma once

#include "Core_APass.h"

namespace Core
{
	class Core_DrawObjectPass : public Core_APass
	{
	public:
		Core_DrawObjectPass(Core_Renderer* pRenderer);

		virtual void BeginPass(Core_Renderer::Drawable pDrawable);
		virtual void DrawPass(Core_Renderer::Drawable pDrawable);
		virtual void EndPass(Core_Renderer::Drawable pDrawable);

	private:
		Core_Renderer* mRenderer;
	};
}

