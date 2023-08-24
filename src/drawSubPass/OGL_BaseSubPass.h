#pragma once

#include "interface/OGL_IDrawSubPass.h"

namespace OGL
{

class OGL_IGraphicsManager;
class OGL_IPipelineStateManager;

class OGL_BaseSubPass : public OGL_IDrawSubPass
{
public:
	OGL_BaseSubPass(OGL_IGraphicsManager* gfxManager, OGL_IPipelineStateManager* pipeManager)
		:mGraphicsManager(gfxManager), mPipelineStateManager(pipeManager)
	{
	}

	virtual void BeginSubPass() override;
	virtual void EndSubPass() override;

protected:
	OGL_IGraphicsManager* mGraphicsManager;
	OGL_IPipelineStateManager* mPipelineStateManager;
};
}