#include "manager/OGL_GraphicsManager.h"
#include "interface/OGL_IPipelineStateManager.h"
#include "OGL_GeometrySubPass.h"

using namespace OGL;

void OGL_GeometrySubPass::Draw(Frame& frame)
{
	const auto& pipelineState = mPipelineStateManager->GetPipelineState("BASIC");
	if (pipelineState)
	{
		mGraphicsManager->SetPipelineState(pipelineState, frame);
		mGraphicsManager->SetShadowMaps(frame);
		mGraphicsManager->DrawBatch(frame);
	}
}
