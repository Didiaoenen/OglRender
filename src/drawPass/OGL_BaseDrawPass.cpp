#include "common/OGL_FrameStructure.h"
#include "manager/OGL_GraphicsManager.h"
#include "interface/OGL_IDrawSubPass.h"
#include "OGL_BaseDrawPass.h"

using namespace OGL;

void OGL_BaseDrawPass::Draw(Frame& frame)
{
	for (const auto& subPass : mDrawSubPasses)
	{
		subPass->BeginSubPass();
		subPass->Draw(frame);
		subPass->EndSubPass();
	}
}

void OGL_BaseDrawPass::BeginPass(Frame& frame)
{
	frame.renderToTexture = mRenderToTexture;
	frame.clearRT = mClearRT;

	mGraphicsManager->BeginPass(frame);
}

void OGL_BaseDrawPass::EndPass(Frame& frame)
{
	mGraphicsManager->EndPass(frame);
}
