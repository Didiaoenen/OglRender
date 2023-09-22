
#include <GL/glew.h>

#include "Render_IndexBuffer.h"

Render::Render_IndexBuffer::Render_IndexBuffer(unsigned int* pData, size_t pElements)
{
	glGenBuffers(1, &mBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pElements * sizeof(unsigned int), pData, GL_STATIC_DRAW);
}

Render::Render_IndexBuffer::Render_IndexBuffer(std::vector<uint32_t>& pData) :
	Render::Render_IndexBuffer(pData.data(), pData.size())
{
}

Render::Render_IndexBuffer::~Render_IndexBuffer()
{
	glDeleteBuffers(1, &mBufferID);
}

void Render::Render_IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
}

void Render::Render_IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t Render::Render_IndexBuffer::GetID()
{
	return mBufferID;
}
