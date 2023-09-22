#pragma once

#include <vector>

#include "Render_Driver.h"

namespace Render
{
	class Render_Framebuffer
	{
	public:
		Render_Framebuffer(uint16_t pWidth = 0, uint16_t pHeight = 0);

		~Render_Framebuffer();

		void Bind();

		void Unbind();

		void Resize(uint16_t pWidth, uint16_t pHeight);

		uint32_t GetID();

		uint32_t GetTextureID();

		uint32_t GetRenderBufferID();

	private:
		uint32_t mBufferID = 0;
		uint32_t mRenderTexture = 0;
		uint32_t mDepthStencilBuffer = 0;
	};
}

