#pragma once

#include <map>
#include <vector>
#include <typeinfo> 
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "base/OGL_Base.h"
#include "common/OGL_GfxConfiguration.h"
#include "interface/OGL_IGraphicsManager.h"

namespace OGL
{

class OGL_Scene;
class OGL_IDrawPass;
class OGL_IDispatchPass;

class OGL_GraphicsManager : public OGL_IGraphicsManager
{
public:
	OGL_GraphicsManager();
	~OGL_GraphicsManager() override = default;

	bool Initialize() override;
	void Finalize() override;

	void Tick(double dt) override;

	void Draw() override;
	void Present() override {}

	void SetPipelineState(const std::shared_ptr<OGL_PipelineState>& pipelineState, const Frame& frame) override {}

	void DrawBatch(const Frame& frame) override {}

	void BeginPass(Frame& frame) override {}
	void EndPass(Frame& frame) override {}

	void BeginCompute() override {}
	void EndCompute() override {}

	void GenerateTexture(Texture2D& texture) override {}
		
	void ReleaseTexture(TextureBase& texture) override {}

	void GenerateCubemapArray(TextureCubeArray& textureArray) override {}

	void GenerateTextureArray(Texture2DArray& textureArray) override {}

	void CreateTextureView(Texture2D& textureView, const TextureArrayBase& textureArray, const uint32_t slice, const uint32_t mip) override {}

	void BeginShadowMap(const int32_t lightIndex, const TextureBase& pShadowMap, const int32_t layerIndex, const Frame& frame) override {}
	void EndShadowMap(const TextureBase& pShadowMap, const int32_t layerIndex, const Frame& frame) override {}

	void SetShadowMaps(const Frame& frame) override {}

	void DrawSkyBox(const Frame& frame) override {}

	void GenerateTextureForWrite(Texture2D& texture) override {}

	void BindTextureForWrite(Texture2D& texture, const uint32_t slotIndex) override {}

	void Dispatch(const uint32_t width, const uint32_t height, const uint32_t depth) override {}

	void DrawFullScreenQuad() override {}

	void MSAAResolve(std::optional<std::reference_wrapper<Texture2D>> target, Texture2D& source) override {}

protected:
	virtual void BeginScene(const OGL_Scene& scene);
	virtual void EndScene();

	virtual void BeginFrame(Frame& frame) {}
	virtual void EndFrame(Frame& frame) {}

	virtual void InitializeGeometries(const OGL_Scene& scene) {}
	virtual void InitializeSkyBox(const OGL_Scene& scene) {}

private:
	void InitConstants() {}
	void CalculateCameraMatrix();
	void CalculateLights();

	void UpdateConstants(double dt);

protected:
	uint64_t mSceneRevision{ 0 };
	uint64_t mFrameIndex{ 0 };

	std::vector<Frame> mFrames;
	std::vector<Ref<OGL_IDispatchPass>> mInitPasses;
	std::vector<Ref<OGL_IDispatchPass>> mDispatchPasses;
	std::vector<Ref<OGL_IDrawPass>> mDrawPasses;

	std::map<std::string, MaterialTexture> mMaterialMap;

	std::vector<TextureBase> mTextures;
	uint32_t mCanvasWidth{ 960 };
	uint32_t mCanvasHeight{ 720 };

private:
	void CreateFrameBuffers();

protected:
	bool mInitialize = false;
};
}

