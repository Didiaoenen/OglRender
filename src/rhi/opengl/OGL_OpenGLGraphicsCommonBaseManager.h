#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "manager/OGL_GraphicsManager.h"

namespace OGL
{
class OGL_OpenGLGraphicsCommonBaseManager : public OGL_GraphicsManager
{
public:
	void Present() final;

	void SetPipelineState(const std::shared_ptr<OGL_PipelineState>& pipelineState, const Frame& frame) final;

	void DrawBatch(const Frame& frame) final;

	void GenerateTexture(Texture2D& texture) final;

	void GenerateCubemapArray(TextureCubeArray& textureArray) final;

	void GenerateTextureArray(Texture2DArray& textureArray) final;
	
	void CreateTextureView(Texture2D& textureView, const TextureArrayBase& textureArray, const uint32_t slice, const uint32_t mip) override {}

	void BeginShadowMap(const int32_t lightIndex, const TextureBase& pShadowMap, const int32_t layerIndex, const Frame& frame) final;
	void EndShadowMap(const TextureBase& pShadowMap, const int32_t layerIndex, const Frame& frame) final;

	void SetShadowMaps(const Frame& frame) final;

	void ReleaseTexture(TextureBase& texture) final;

	void DrawSkyBox(const Frame& frame) final;

	void GenerateTextureForWrite(Texture2D& texture) final;

	void BindTextureForWrite(Texture2D& texture, const uint32_t slotIndex) final;

	void Dispatch(const uint32_t width, const uint32_t height, const uint32_t depth) final;

	void DrawFullScreenQuad() final;

	void MSAAResolve(std::optional<std::reference_wrapper<Texture2D>> target, Texture2D& source) final;

protected:
	void EndScene() final;

	void BeginFrame(Frame& frame) override;
	void EndFrame(Frame& frame) override;

	void InitializeGeometries(const OGL_Scene& scene) final;
	void InitializeSkyBox(const OGL_Scene& scene) final;
	
	void SetPerFrameConstants(const DrawFrameContext& context);
	void SetPerBatchConstants(const DrawBatchContext& context);
	void SetLightInfo(const LightInfo& lightInfo);

	bool SetShaderParameter(const std::string& paramName, const glm::mat4& param);
	bool SetShaderParameter(const std::string& paramName, const glm::mat4* param, const int32_t count);
	bool SetShaderParameter(const std::string& paramName, const glm::vec4& param);
	bool SetShaderParameter(const std::string& paramName, const glm::vec3& param);
	bool SetShaderParameter(const std::string& paramName, const glm::vec2& param);
	bool SetShaderParameter(const std::string& paramName, const float param);
	bool SetShaderParameter(const std::string& paramName, const int32_t param);
	bool SetShaderParameter(const std::string& paramName, const uint32_t param);
	bool SetShaderParameter(const std::string& paramName, const bool param);

	virtual void GetOpenGLTextureFormat(const PixelFormat pixelFormat, uint32_t& format, uint32_t& internalFormat, uint32_t& type) = 0;

	virtual void GetOpenGLTextureFormat(const CompressedFormat compressedFormat, uint32_t& format, uint32_t& internalFormat, uint32_t& type) = 0;

private:
	uint32_t mShadowMapFrameBuffer;
	uint32_t mCurrentShader;
	uint32_t mUBOLightInfo[OGL_GfxConfiguration::kMaxInFlightFrameCount] = { 0 };
	uint32_t mUBODrawFrameConstant[OGL_GfxConfiguration::kMaxInFlightFrameCount] = { 0 };
	uint32_t mUBODrawBatchConstant[OGL_GfxConfiguration::kMaxInFlightFrameCount] = { 0 };
	uint32_t mUBOShadowMatricesConstant[OGL_GfxConfiguration::kMaxInFlightFrameCount] = { 0 };

	struct OpenGLDrawBatchContext : public DrawBatchContext 
	{
		uint32_t vao{ 0 };
		uint32_t mode{ 0 };
		uint32_t type{ 0 };
		int32_t count{ 0 };
	};

	std::vector<uint32_t> mBuffers;

	OpenGLDrawBatchContext mSkyBoxDrawBatchContext;
	OpenGLDrawBatchContext mTerrainDrawBatchContext;
};
}

