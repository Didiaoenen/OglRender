#pragma once

#include <stdint.h>

#include "manager/OGL_PipelineStateManager.h"

namespace OGL
{
struct OGL_OpenGLPipelineState : public OGL_PipelineState
{
	uint32_t shaderProgram = 0;
	OGL_OpenGLPipelineState(OGL_PipelineState& rhs) : OGL_PipelineState(rhs) {}
	OGL_OpenGLPipelineState(OGL_PipelineState&& rhs) : OGL_PipelineState(std::move(rhs)) {}
};

class OGL_OpenGLPipelineStateCommonBaseManager : public OGL_PipelineStateManager 
{
public:
	using OGL_PipelineStateManager::OGL_PipelineStateManager;
	virtual ~OGL_OpenGLPipelineStateCommonBaseManager() = default;

protected:
	bool InitializePipelineState(OGL_PipelineState** pipelineState) final;
	void DestroyPipelineState(OGL_PipelineState& pipelineState) final;
};
}

