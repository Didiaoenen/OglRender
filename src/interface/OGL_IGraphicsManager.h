#pragma once

#include <optional>

#include "common/OGL_FrameStructure.h"
#include "interface/OGL_IRuntimeModule.h"

namespace OGL
{

struct OGL_PipelineState;

class OGL_IGraphicsManager : public OGL_IRuntimeModule
{
public:
	OGL_IGraphicsManager() = default;
	virtual ~OGL_IGraphicsManager() = default;

	virtual void Draw() = 0;
	virtual void Present() = 0;

	virtual void SetPipelineState(const std::shared_ptr<OGL_PipelineState>& pipelineState, const Frame& frame) = 0;

	virtual void DrawBatch(const Frame& frame) = 0;

	virtual void BeginPass(Frame& frame) = 0;
	virtual void EndPass(Frame& frame) = 0;

	virtual void BeginCompute() = 0;
	virtual void EndCompute() = 0;

	virtual void GenerateTexture(Texture2D& texture) = 0;
		
	virtual void ReleaseTexture(TextureBase& texture) = 0;

	virtual void CreateTextureView(Texture2D& textureView, const TextureArrayBase& textureArray, const uint32_t slice, const uint32_t mip) = 0;

	virtual void GenerateCubemapArray(TextureCubeArray& textureArray) = 0;

	virtual void GenerateTextureArray(Texture2DArray& textureArray) = 0;

	virtual void BeginShadowMap(const int32_t lightIndex, const TextureBase& pShadowMap, const int32_t layerIndex, const Frame& frame) = 0;
	virtual void EndShadowMap(const TextureBase& pShadowMap, const int32_t layerIndex, const Frame& frame) = 0;

	virtual void SetShadowMaps(const Frame& frame) = 0;

	virtual void DrawSkyBox(const Frame& frame) = 0;

	virtual void GenerateTextureForWrite(Texture2D& texture) = 0;

	virtual void BindTextureForWrite(Texture2D& texture, const uint32_t slotIndex) = 0;

	virtual void Dispatch(const uint32_t width, const uint32_t height, const uint32_t depth) = 0;

	virtual void DrawFullScreenQuad() = 0;

	virtual void MSAAResolve(std::optional<std::reference_wrapper<Texture2D>> target, Texture2D& source) = 0;
};
}
