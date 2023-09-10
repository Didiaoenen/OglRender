#pragma once

#include <GL/glew.h>

#include "Render_VertexBuffer.h"

namespace Render
{
	template <class T>
	inline Render_VertexBuffer<T>::Render_VertexBuffer(T* p_data, size_t p_elements)
	{
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, p_elements * sizeof(T), p_data, GL_STATIC_DRAW);
	}

	template<class T>
	inline Render_VertexBuffer<T>::Render_VertexBuffer(std::vector<T>& p_data) : Render_VertexBuffer(p_data.data(), p_data.size())
	{
	}

	template<class T>
	inline Render_VertexBuffer<T>::~Render_VertexBuffer()
	{
		glDeleteBuffers(1, &m_bufferID);
	}

	template <class T>
	inline void Render_VertexBuffer<T>::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
	}

	template <class T>
	inline void Render_VertexBuffer<T>::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <class T>
	inline uint32_t Render_VertexBuffer<T>::GetID()
	{
		return m_bufferID;
	}
}