#pragma once

namespace Render
{
	struct Render_PipelineState
	{
		int blendSrc;
		int blendDst;
		int depthTest;
		int depthWrite;
		int colorMask;
		int culling;
	};
}