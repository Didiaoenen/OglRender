
#include <GL/glew.h>

#include "Render_Shader.h"

void Render::Render_Shader::Bind() const
{
}

void Render::Render_Shader::Unbind() const
{
}

void Render::Render_Shader::SetUniformInt(const std::string& pName, int pValue)
{
}

void Render::Render_Shader::SetUniformFloat(const std::string& pName, float pValue)
{
}

void Render::Render_Shader::SetUniformVec2(const std::string& pName, const glm::vec2& p_vec2)
{
}

void Render::Render_Shader::SetUniformVec3(const std::string& pName, const glm::vec3& p_vec3)
{
}

void Render::Render_Shader::SetUniformVec4(const std::string& pName, const glm::vec4& p_vec4)
{
}

void Render::Render_Shader::SetUniformMat4(const std::string& pName, const glm::mat4& p_mat4)
{
}

int Render::Render_Shader::GetUniformInt(const std::string& pName)
{
	return 0;
}

float Render::Render_Shader::GetUniformFloat(const std::string& pName)
{
	return 0.0f;
}

glm::vec2 Render::Render_Shader::GetUniformVec2(const std::string& pName)
{
	return glm::vec2();
}

glm::vec3 Render::Render_Shader::GetUniformVec3(const std::string& pName)
{
	return glm::vec3();
}

glm::vec4 Render::Render_Shader::GetUniformVec4(const std::string& pName)
{
	return glm::vec4();
}

glm::mat4 Render::Render_Shader::GetUniformMat4(const std::string& pName)
{
	return glm::mat4();
}

const Render::Render_UniformInfo* Render::Render_Shader::GetUniformInfo(const std::string& pName) const
{
	return nullptr;
}

void Render::Render_Shader::QueryUniforms()
{
}

Render::Render_Shader::Render_Shader(const std::string pPath, uint32_t pId) :
	path(pPath), id(pId)
{
}

Render::Render_Shader::~Render_Shader()
{
}

bool Render::Render_Shader::IsEngineUBOMember(const std::string& p_uniformName)
{
	return false;
}

uint32_t Render::Render_Shader::GetUniformLocation(const std::string& name)
{
	return 0;
}
