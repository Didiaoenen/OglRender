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
		Render_ShaderStorageBuffer(EAccessSpecifier p_accessSpecifier);

		~Render_ShaderStorageBuffer();

		void Bind(uint32_t p_bindingPoint);

		void Unbind();

		template<typename T>
		void SendBlocks(T* p_data, size_t p_size);

	private:
		uint32_t m_bufferID;
		uint32_t m_bindingPoint = 0;
	};
}

#include "Render_ShaderStorageBuffer.inl"