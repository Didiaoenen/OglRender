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
		Render_UniformBuffer(size_t pSize, uint32_t p_bindingPoint = 0, uint32_t p_offset = 0, EAccessSpecifier p_accessSpecifier = EAccessSpecifier::DYNAMIC_DRAW);

		~Render_UniformBuffer();

		void Bind();

		void Unbind();

		template<typename T>
		void SetSubData(const T& p_data, size_t p_offset);

		template<typename T>
		void SetSubData(const T& p_data, std::reference_wrapper<size_t> p_offsetInOut);

		uint32_t GetID() const;

		static void BindBlockToShader(Render_Shader& p_shader, uint32_t p_uniformBlockLocation, uint32_t p_bindingPoint = 0);

		static void BindBlockToShader(Render_Shader& p_shader, const std::string& pName, uint32_t p_bindingPoint = 0);

		static uint32_t GetBlockLocation(Render_Shader& p_shader, const std::string& pName);

	private:
		uint32_t m_bufferID;
	};
}

#include "Render_UniformBuffer.inl"