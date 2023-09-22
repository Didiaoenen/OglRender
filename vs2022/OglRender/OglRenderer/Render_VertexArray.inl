#pragma once

#include <GL/glew.h>

#include "Render_VertexArray.h"

namespace Render
{
	template <class T>
	inline void Render_VertexArray::BindAttribute(uint32_t pAttribute, Render_VertexBuffer<T>& pVertexBuffer, EType pType, uint64_t pCount, uint64_t pStride, intptr_t pOffset)
	{
		Bind();
		pVertexBuffer.Bind();
		glEnableVertexAttribArray(pAttribute);
		glVertexAttribPointer(static_cast<GLuint>(pAttribute), static_cast<GLint>(pCount), static_cast<GLenum>(pType), GL_FALSE, static_cast<GLsizei>(pStride), reinterpret_cast<const GLvoid*>(pOffset));
	}
}