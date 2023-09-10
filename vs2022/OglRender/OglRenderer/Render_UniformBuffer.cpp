
#include <GL/glew.h>

#include "Render_UniformBuffer.h"

Render::Render_UniformBuffer::Render_UniformBuffer(size_t p_size, uint32_t p_bindingPoint, uint32_t p_offset, EAccessSpecifier p_accessSpecifier)
{
}

Render::Render_UniformBuffer::~Render_UniformBuffer()
{
}

void Render::Render_UniformBuffer::Bind()
{
}

void Render::Render_UniformBuffer::Unbind()
{
}

uint32_t Render::Render_UniformBuffer::GetID() const
{
	return 0;
}

void Render::Render_UniformBuffer::BindBlockToShader(Render_Shader& p_shader, uint32_t p_uniformBlockLocation, uint32_t p_bindingPoint)
{
}

void Render::Render_UniformBuffer::BindBlockToShader(Render_Shader& p_shader, const std::string& p_name, uint32_t p_bindingPoint)
{
}

uint32_t Render::Render_UniformBuffer::GetBlockLocation(Render_Shader& p_shader, const std::string& p_name)
{
	return 0;
}
