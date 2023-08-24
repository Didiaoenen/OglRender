#pragma once

#include <stdint.h>

namespace OGL
{
struct OGL_GfxConfiguration
{
	explicit OGL_GfxConfiguration()
	{

	}

	uint32_t screenWidth{ 960 };
	uint32_t screenHeight{ 720 };

	uint32_t msaaSamples{ 0 };

	static const uint32_t kMaxInFlightFrameCount{2};
	static const uint32_t kMaxShadowMapCount{8};

	static const uint32_t kShadowMapWidth = 512;
	static const uint32_t kShadowMapHeight = 512;

	bool fixOpenGLPerspectiveMatrix = false;
};
}
