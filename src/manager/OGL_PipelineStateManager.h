#pragma once

#include <map>
#include <string>

#include "base/OGL_Base.h"
#include "interface/OGL_IPipelineStateManager.h"

namespace OGL
{
class OGL_PipelineStateManager : public OGL_IPipelineStateManager
{
public:
	~OGL_PipelineStateManager() override;

	bool Initialize() override;
	void Finalize() override;
	void Tick() override {}

	void RegisterPipelineState(OGL_PipelineState& pipelineState) override;
	void UnregisterPipelineState(OGL_PipelineState& pipelineState) override;
	
	void Clear() override;

	const Ref<OGL_PipelineState> GetPipelineState(std::string name) const final;
	
protected:
	virtual bool InitializePipelineState(OGL_PipelineState** ppPipelineState) { return true; }

	virtual void DestroyPipelineState(OGL_PipelineState& pipelineState) {}

protected:
	std::map<std::string, Ref<OGL_PipelineState>> mPipelineStates;
};
}
