#pragma once

#include <vector>

#include "Render_Driver.h"
#include "Render_EAccessSpecifier.h"

namespace Render
{
	class Render_Shader;

	class Render_ShaderStorageBuffer
	{
	public:
		Render_ShaderStorageBuffer(EAccessSpecifier pAccessSpecifier);

		~Render_ShaderStorageBuffer();

		void Bind(uint32_t pBindingPoint);

		void Unbind();

		template<typename T>
		void SendBlocks(T* pData, size_t pSize);

	private:
		uint32_t mBufferID{ 0 };
		uint32_t mBindingPoint{ 0 };
	};
}

#include "Render_ShaderStorageBuffer.inl"