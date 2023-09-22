#pragma once

#include <GL/glew.h>

#include "Render_VertexBuffer.h"

namespace Render
{
	template <class T>
	inline Render_VertexBuffer<T>::Render_VertexBuffer(T* pData, size_t pElements)
	{
		glGenBuffers(1, &mBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		glBufferData(GL_ARRAY_BUFFER, pElements * sizeof(T), pData, GL_STATIC_DRAW);
	}

	template<class T>
	inline Render_VertexBuffer<T>::Render_VertexBuffer(std::vector<T>& pData) : 
		Render_VertexBuffer(pData.data(), pData.size())
	{
	}

	template<class T>
	inline Render_VertexBuffer<T>::~Render_VertexBuffer()
	{
		glDeleteBuffers(1, &mBufferID);
	}

	template <class T>
	inline void Render_VertexBuffer<T>::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
	}

	template <class T>
	inline void Render_VertexBuffer<T>::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <class T>
	inline uint32_t Render_VertexBuffer<T>::GetID()
	{
		return mBufferID;
	}
}