#pragma once

#include "OGL_BaseDrawPass.h"

namespace OGL
{

class OGL_IGraphicsManager;
class OGL_IPipelineStateManager;

class OGL_OverlayPass : public OGL_BaseDrawPass
{
public:
    OGL_OverlayPass(OGL_IGraphicsManager* pGfxMgr, OGL_IPipelineStateManager* pPipeMgr)
        : OGL_BaseDrawPass(pGfxMgr, pPipeMgr)
    {
        //mDrawSubPasses.push_back(std::make_shared<GuiSubPass>(mGraphicsManager, mPipelineStateManager));
        //mDrawSubPasses.push_back(std::make_shared<DebugOverlaySubPass>(mGraphicsManager, mPipelineStateManager));
    }
};
}

