#pragma once

#include "OGL_BaseDrawPass.h"

namespace OGL
{
class OGL_ForwardGeometryPass : public OGL_BaseDrawPass
{
public:
    OGL_ForwardGeometryPass(OGL_IGraphicsManager* gfxManger, OGL_IPipelineStateManager* pipeManager);
};
}

