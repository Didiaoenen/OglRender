#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "base/OGL_Base.h"
#include "common/OGL_Define.h"

namespace OGL
{

class OGL_Entity;

struct DrawFrameContext : PerFrameConstants, FrameTextures
{

};

struct DrawBatchContext : PerBatchConstants
{
	virtual ~DrawBatchContext() = default;

	int32_t batchIndex{ 0 };
	MaterialTexture material;
	Ref<OGL_Entity> entity;
	bool animator;
};

struct Frame : GlobalTexture
{
	int32_t frameIndex{ 0 };
	LightInfo lightInfo{};
	DrawFrameContext frameContext{};
	std::vector<Ref<DrawBatchContext>> batchContexts{};
	glm::vec4 clearColor {0.2f, 0.3f, 0.4f, 1.0f};
	std::vector<Texture2D> colorTextures;
	Texture2D depthTexture;
	GLuint frameBuffer;
	bool renderToTexture = false;
	bool enableMSAA = false;
	bool clearRT = true;
};
}
