#pragma once

#include <vector>

#include "Render_Driver.h"

namespace Render
{
	class Render_Framebuffer
	{
	public:
		Render_Framebuffer(uint16_t p_width = 0, uint16_t p_height = 0);

		~Render_Framebuffer();

		void Bind();

		void Unbind();

		void Resize(uint16_t p_width, uint16_t p_height);

		uint32_t GetID();

		uint32_t GetTextureID();

		uint32_t GetRenderBufferID();

	private:
		uint32_t m_bufferID = 0;
		uint32_t m_renderTexture = 0;
		uint32_t m_depthStencilBuffer = 0;
	};
}

