#pragma once

#include <string>
#include <vector>

#include "Core_Renderer.h"

namespace Render
{
	class Render_Program;
}

namespace Core
{
	class Core_APass
	{
	public:
		virtual void BeginPass(Core_Renderer::Drawable pDrawable) = 0;
		virtual void DrawPass(Core_Renderer::Drawable pDrawable) = 0;
		virtual void EndPass(Core_Renderer::Drawable pDrawable) = 0;

	protected:
		std::vector<std::string> mProgramNames;
	};
}

