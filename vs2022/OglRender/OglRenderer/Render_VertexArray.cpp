#include "Render_VertexArray.h"

Render::Render_VertexArray::Render_VertexArray()
{
	glGenVertexArrays(1, &mBufferID);
	glBindVertexArray(mBufferID);
}

Render::Render_VertexArray::~Render_VertexArray()
{
	glDeleteVertexArrays(1, &mBufferID);
}

void Render::Render_VertexArray::Bind()
{
	glBindVertexArray(mBufferID);
}

void Render::Render_VertexArray::Unbind()
{
	glBindVertexArray(0);
}

GLint Render::Render_VertexArray::GetID()
{
	return mBufferID;
}
