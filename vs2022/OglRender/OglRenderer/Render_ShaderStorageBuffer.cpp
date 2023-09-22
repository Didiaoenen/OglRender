
#include <GL/glew.h>

#include "Render_ShaderStorageBuffer.h"

Render::Render_ShaderStorageBuffer::Render_ShaderStorageBuffer(EAccessSpecifier pAccessSpecifier)
{
	glGenBuffers(1, &mBufferID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, static_cast<GLenum>(pAccessSpecifier));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mBufferID);
}

Render::Render_ShaderStorageBuffer::~Render_ShaderStorageBuffer()
{
	glDeleteBuffers(1, &mBufferID);
}

void Render::Render_ShaderStorageBuffer::Bind(uint32_t pBindingPoint)
{
	mBindingPoint = pBindingPoint;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, pBindingPoint, mBufferID);
}

void Render::Render_ShaderStorageBuffer::Unbind()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mBindingPoint, 0);
}
