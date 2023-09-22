#pragma once

#include <vector>

#include "Render_Driver.h"

namespace Render
{
	class Render_IndexBuffer
	{
	public:

		Render_IndexBuffer(unsigned int* pData, size_t pElements);

		Render_IndexBuffer(std::vector<uint32_t>& pData);

		~Render_IndexBuffer();

		void Bind();

		void Unbind();

		uint32_t GetID();

	private:
		uint32_t mBufferID{ 0 };
	};
}

