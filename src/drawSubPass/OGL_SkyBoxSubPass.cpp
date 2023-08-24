#include "manager/OGL_GraphicsManager.h"
#include "interface/OGL_IPipelineStateManager.h"
#include "OGL_SkyBoxSubPass.h"

using namespace OGL;

void OGL_SkyBoxSubPass::Draw(Frame& frame)
{
	const auto& pipelineState = mPipelineStateManager->GetPipelineState("");
	if (pipelineState)
	{
		mGraphicsManager->SetPipelineState(pipelineState, frame);
		mGraphicsManager->DrawSkyBox(frame);
	}
}
