#pragma once

#include <vector>
#include <memory>

#include "base/OGL_Base.h"
#include "interface/OGL_IDrawPass.h"

namespace OGL
{

struct Frame;
class OGL_IDrawSubPass;
class OGL_IGraphicsManager;
class OGL_IPipelineStateManager;

class OGL_BaseDrawPass : public OGL_IDrawPass
{
public:
    OGL_BaseDrawPass(OGL_IGraphicsManager* gfxManager, OGL_IPipelineStateManager* pipeManager)
        : mGraphicsManager(gfxManager), mPipelineStateManager(pipeManager)
    {
    }

    void Draw(Frame& frame) override;

    void BeginPass(Frame& frame) override;
    void EndPass(Frame& frame) override;
        
    void EnableRenderToTexture() { mRenderToTexture = true; }
    void DisableRenderToTexture() { mRenderToTexture = false; }

protected:
    OGL_BaseDrawPass() = default;

protected:
    OGL_IGraphicsManager* mGraphicsManager;
    OGL_IPipelineStateManager* mPipelineStateManager;
    std::vector<Ref<OGL_IDrawSubPass>> mDrawSubPasses;
    bool mRenderToTexture = false;
    bool mClearRT = false;
};
}

