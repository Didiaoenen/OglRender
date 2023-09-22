#pragma once

#include <vector>

#include "Render_Driver.h"

namespace Render
{
	template<typename T>
	class Render_VertexBuffer
	{
	public:
		Render_VertexBuffer(T* pData, size_t pElements);

		Render_VertexBuffer(std::vector<T>& pData);

		~Render_VertexBuffer();

		void Bind();

		void Unbind();

		uint32_t GetID();

	private:
		uint32_t mBufferID{ 0 };
	};
}

#include "Render_VertexBuffer.inl"