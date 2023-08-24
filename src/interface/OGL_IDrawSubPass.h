#pragma once

#include "common/OGL_FrameStructure.h"
#include "interface/OGL_ISubPass.h"

namespace OGL
{
class OGL_IDrawSubPass : public OGL_ISubPass
{
public:
	virtual void Draw(Frame& frame) = 0;
};
}