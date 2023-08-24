#include "manager/OGL_GraphicsManager.h"
#include "OGL_BaseDispatchPass.h"

using namespace OGL;

void OGL_BaseDispatchPass::BeginPass([[maybe_unused]] Frame& frame)
{
	mGraphicsManager->BeginCompute();
}

void OGL_BaseDispatchPass::EndPass([[maybe_unused]] Frame& frame)
{
	mGraphicsManager->EndCompute();
}
