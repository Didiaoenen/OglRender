#pragma once

#include <vector>
#include <string>

#include "Render_Driver.h"
#include "Render_EAccessSpecifier.h"

namespace Render
{
	class Render_Shader;

	class Render_UniformBuffer
	{
	public:
		Render_UniformBuffer(size_t pSize, uint32_t pBindingPoint = 0, uint32_t pOffset = 0, EAccessSpecifier pAccessSpecifier = EAccessSpecifier::DYNAMIC_DRAW);

		~Render_UniformBuffer();

		void Bind();

		void Unbind();

		template<typename T>
		void SetSubData(const T& pData, size_t pOffset);

		template<typename T>
		void SetSubData(const T& pData, std::reference_wrapper<size_t> pOffsetInOut);

		uint32_t GetID() const;

		static void BindBlockToShader(Render_Shader& pShader, uint32_t pUniformBlockLocation, uint32_t pBindingPoint = 0);

		static void BindBlockToShader(Render_Shader& pShader, const std::string& pName, uint32_t pBindingPoint = 0);

		static uint32_t GetBlockLocation(Render_Shader& pShader, const std::string& pName);

	private:
		uint32_t mBufferID{ 0 };
	};
}

#include "Render_UniformBuffer.inl"