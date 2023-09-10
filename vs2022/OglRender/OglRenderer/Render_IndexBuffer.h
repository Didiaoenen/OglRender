#pragma once

#include <vector>

#include "Render_Driver.h"

namespace Render
{
	class Render_IndexBuffer
	{
	public:

		Render_IndexBuffer(unsigned int* p_data, size_t p_elements);

		Render_IndexBuffer(std::vector<uint32_t>& p_data);

		~Render_IndexBuffer();

		void Bind();

		void Unbind();

		uint32_t GetID();

	private:
		uint32_t m_bufferID;
	};
}

