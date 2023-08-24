#pragma once

#include "interface/OGL_IDispatchPass.h"

namespace OGL
{
struct Frame;
class OGL_IGraphicsManager;
class OGL_IPipelineStateManager;

class OGL_BaseDispatchPass : public OGL_IDispatchPass
{
public:
	OGL_BaseDispatchPass(OGL_IGraphicsManager* gfxManager, OGL_IPipelineStateManager* pipeManager)
		: mGraphicsManager(gfxManager), mPipelineStateManager(pipeManager)
	{
	}
		
	void BeginPass(Frame& frame) override;
	void EndPass(Frame& frame) override;

protected:
	OGL_IGraphicsManager* mGraphicsManager;
	OGL_IPipelineStateManager* mPipelineStateManager;
};
}