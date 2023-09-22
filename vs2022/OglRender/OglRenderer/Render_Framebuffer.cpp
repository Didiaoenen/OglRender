
#include <GL/glew.h>

#include "Render_Framebuffer.h"

Render::Render_Framebuffer::Render_Framebuffer(uint16_t pWidth, uint16_t pHeight)
{
	glGenFramebuffers(1, &mBufferID);
	glGenTextures(1, &mRenderTexture);
	glGenRenderbuffers(1, &mDepthStencilBuffer);

	glBindTexture(GL_TEXTURE_2D, mRenderTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	Bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderTexture, 0);
	Unbind();

	Resize(pWidth, pHeight);
}

Render::Render_Framebuffer::~Render_Framebuffer()
{
	glDeleteBuffers(1, &mBufferID);
	glDeleteTextures(1, &mRenderTexture);
	glDeleteRenderbuffers(1, &mDepthStencilBuffer);
}

void Render::Render_Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
}

void Render::Render_Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::Render_Framebuffer::Resize(uint16_t pWidth, uint16_t pHeight)
{
	glBindTexture(GL_TEXTURE_2D, mRenderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pWidth, pHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, pWidth, pHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	Bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER, mDepthStencilBuffer);
	Unbind();
}

uint32_t Render::Render_Framebuffer::GetID()
{
	return mBufferID;
}

uint32_t Render::Render_Framebuffer::GetTextureID()
{
	return mRenderTexture;
}

uint32_t Render::Render_Framebuffer::GetRenderBufferID()
{
	return mDepthStencilBuffer;
}
