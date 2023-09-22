#pragma once

#include <GL/glew.h>

#include "Render_UniformBuffer.h"

namespace Render
{
	template<typename T>
	inline void Render_UniformBuffer::SetSubData(const T& pData, size_t pOffsetInOut)
	{
		Bind();
		glBufferSubData(GL_UNIFORM_BUFFER, pOffsetInOut, sizeof(T), std::addressof(pData));
		Unbind();
	}

	template<typename T>
	inline void Render_UniformBuffer::SetSubData(const T& pData, std::reference_wrapper<size_t> pOffsetInOut)
	{
		Bind();
		size_t dataSize = sizeof(T);
		glBufferSubData(GL_UNIFORM_BUFFER, pOffsetInOut.get(), dataSize, std::addressof(pData));
		pOffsetInOut.get() += dataSize;
		Unbind();
	}
}