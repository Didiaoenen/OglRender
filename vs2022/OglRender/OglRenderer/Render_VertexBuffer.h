#pragma once

#include <vector>

#include "Render_Driver.h"

namespace Render
{
	template<typename T>
	class Render_VertexBuffer
	{
	public:
		Render_VertexBuffer(T* p_data, size_t p_elements);

		Render_VertexBuffer(std::vector<T>& p_data);

		~Render_VertexBuffer();

		void Bind();

		void Unbind();

		uint32_t GetID();

	private:
		uint32_t m_bufferID;
	};
}

#include "Render_VertexBuffer.inl"