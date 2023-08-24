#pragma once

#include "common/OGL_FrameStructure.h"

namespace OGL
{
class OGL_IPass
{
public:
	virtual ~OGL_IPass() = default;

	virtual void BeginPass(Frame& frame) = 0;
	virtual void EndPass(Frame& frame) = 0;
};
}
