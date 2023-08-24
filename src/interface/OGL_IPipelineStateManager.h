#pragma once

#include <string>

#include "base/OGL_Base.h"
#include "common/OGL_Define.h"
#include "interface/OGL_IRuntimeModule.h"

namespace OGL
{
struct OGL_PipelineState
{
	virtual ~OGL_PipelineState() = default;

	std::string pipelineName;

	std::string vertexShaderName;
	std::string fragmentShaderName;
	std::string geometryShaderName;

	CullFace cullFace{ CullFace::BACK };
	DepthTest depthTest{ DepthTest::ALWAYS };
	StencilTest stencilTest{ StencilTest::NONE };
	PixelFormat pixelFormat{ PixelFormat::RGBA8 };
	uint32_t sampleCount{ 1 };
	
	bool bWriteDepth{ true };

	DrawFlag flag;
};

class OGL_IPipelineStateManager : public OGL_IRuntimeModule
{
public:
	virtual void RegisterPipelineState(OGL_PipelineState& pipelineState) = 0;
	virtual void UnregisterPipelineState(OGL_PipelineState& pipelineState) = 0;
	virtual void Clear() = 0;

	[[nodiscard]] virtual const Ref<OGL_PipelineState> GetPipelineState(std::string name) const = 0;
};
}