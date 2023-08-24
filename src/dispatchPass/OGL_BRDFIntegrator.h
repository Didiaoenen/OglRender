#pragma once

#include "OGL_BaseDispatchPass.h"

namespace OGL
{
class OGL_BRDFIntegrator : public OGL_BaseDispatchPass
{
public:
	using OGL_BaseDispatchPass::OGL_BaseDispatchPass;
	void Dispatch(Frame& frame) final;
};
}
