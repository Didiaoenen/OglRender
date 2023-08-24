#pragma once

#include "OGL_BaseDrawPass.h"

namespace OGL
{

struct Frame;

class OGL_ShadowMapPass : public OGL_BaseDrawPass
{
public:
	using OGL_BaseDrawPass::OGL_BaseDrawPass;
	~OGL_ShadowMapPass() override = default;

	void Draw(Frame& frame) final;

	void BeginPass(Frame& frame) override {}
	void EndPass(Frame& frame) override {}
};
}

