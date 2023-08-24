#include "common/OGL_GfxConfiguration.h"
#include "interface/OGL_IApplication.h"
#include "OGL_PipelineStateManager.h"

using namespace OGL;

#define VS_BASIC_SOURCE_FILE "basic.vert"
#define PS_BASIC_SOURCE_FILE "basic.frag"

OGL_PipelineStateManager::~OGL_PipelineStateManager()
{
}

bool OGL_PipelineStateManager::Initialize()
{
	OGL_PipelineState pipelineState;
	pipelineState.pipelineName = "BASIC";
	pipelineState.vertexShaderName = VS_BASIC_SOURCE_FILE;
	pipelineState.fragmentShaderName = PS_BASIC_SOURCE_FILE;
	pipelineState.bWriteDepth = true;
	pipelineState.depthTest = DepthTest::LEQUAL;
	pipelineState.stencilTest = StencilTest::NONE;
	pipelineState.cullFace = CullFace::BACK;
	pipelineState.sampleCount = mApp->GetConfiguration().msaaSamples;
	pipelineState.flag = DrawFlag::NONE;
	RegisterPipelineState(pipelineState);

	return true;
}

void OGL_PipelineStateManager::Finalize()
{
	Clear();
}

void OGL_PipelineStateManager::RegisterPipelineState(OGL_PipelineState& pipelineState)
{
	OGL_PipelineState* pRef;
	pRef = &pipelineState;
	if (InitializePipelineState(&pRef))
	{
		mPipelineStates.emplace(pipelineState.pipelineName, pRef);
	}
}

void OGL_PipelineStateManager::UnregisterPipelineState(OGL_PipelineState& pipelineState)
{
	mPipelineStates.erase(pipelineState.pipelineName);
}

void OGL_PipelineStateManager::Clear()
{
}

const Ref<OGL_PipelineState> OGL_PipelineStateManager::GetPipelineState(std::string name) const
{
	const auto& it = mPipelineStates.find(name);
	if (it != mPipelineStates.end())
	{
		return it->second;
	}

	return nullptr;
}
