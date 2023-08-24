#pragma once

#include "OGL_BaseSubPass.h"

namespace OGL
{

struct Frame;

class OGL_GeometrySubPass : public OGL_BaseSubPass
{
public:
	using OGL_BaseSubPass::OGL_BaseSubPass;
	void Draw(Frame& frame) final;
};
}