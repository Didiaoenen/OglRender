#pragma once

#include "interface/OGL_IPass.h"

namespace OGL
{
class OGL_IDrawPass : public OGL_IPass
{
public:
	virtual void Draw(Frame& frame) = 0;
};
}