#pragma once

#include "Render_UniformBuffer.h"
#include "Render_ShaderStorageBuffer.h"

namespace Render
{
	template<typename T>
	inline void Render_ShaderStorageBuffer::SendBlocks(T* pData, size_t pSize)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, pSize, pData, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}