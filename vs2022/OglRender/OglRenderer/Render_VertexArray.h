#pragma once

#include "Render_VertexBuffer.h"

namespace Render
{
	enum class EType
	{
		BYTE = 0x1400,
		UNISGNED_BYTE = 0x1401,
		SHORT = 0x1402,
		UNSIGNED_SHORT = 0x1403,
		INT = 0x1404,
		UNSIGNED_INT = 0x1405,
		FLOAT = 0x1406,
		DOUBLE = 0x140A
	};

	class Render_VertexArray
	{
	public:
		Render_VertexArray();

		~Render_VertexArray();

		template <class T>
		void BindAttribute(uint32_t p_attribute, Render_VertexBuffer<T>& p_vertexBuffer, EType p_type, uint64_t p_count, uint64_t p_stride, intptr_t p_offset);

		void Bind();

		void Unbind();

		GLint GetID();

	private:
		GLuint m_bufferID;
	};
}

#include "Render_VertexArray.inl"