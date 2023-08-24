#include "base/OGL_Base.h"
#include "drawSubpass/OGL_SkyBoxSubPass.h"
#include "drawSubpass/OGL_GeometrySubPass.h"
#include "OGL_ForwardGeometryPass.h"

using namespace OGL;

OGL_ForwardGeometryPass::OGL_ForwardGeometryPass(OGL_IGraphicsManager* gfxManager, OGL_IPipelineStateManager* pipeManager)
    : OGL_BaseDrawPass(gfxManager, pipeManager)
{
    mClearRT = true;
    mDrawSubPasses.push_back(CreateRef<OGL_SkyBoxSubPass>(mGraphicsManager, mPipelineStateManager));
    mDrawSubPasses.push_back(CreateRef<OGL_GeometrySubPass>(mGraphicsManager, mPipelineStateManager));
}