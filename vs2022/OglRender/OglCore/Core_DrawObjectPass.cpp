
#include <OglRenderer/Render_Program.h>

#include "Core_DrawObjectPass.h"

Core::Core_DrawObjectPass::Core_DrawObjectPass(Core_Renderer* pRenderer) :
	mRenderer(pRenderer)
{
	mProgramNames.push_back("_1");
	mProgramNames.push_back("_2");
	mProgramNames.push_back("_3");
}

void Core::Core_DrawObjectPass::BeginPass(Core_Renderer::Drawable pDrawable)
{

}

void Core::Core_DrawObjectPass::DrawPass(Core_Renderer::Drawable pDrawable)
{
	for (const auto& programName : mProgramNames)
	{
		mRenderer->DrawDrawable(pDrawable, programName);
	}
}

void Core::Core_DrawObjectPass::EndPass(Core_Renderer::Drawable pDrawable)
{
}
