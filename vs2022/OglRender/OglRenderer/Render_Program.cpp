#include <GL/glew.h>

#include "Render_Texture.h"
#include "Render_Program.h"

void Render::Render_Program::Bind() const
{
	glUseProgram(mId);
}

void Render::Render_Program::Unbind() const
{
	glUseProgram(0);
}

void Render::Render_Program::SetUniformInt(const std::string& pName, int pValue)
{
	glUniform1i(GetUniformLocation(pName), pValue);
}

void Render::Render_Program::SetUniformFloat(const std::string& pName, float pValue)
{
	glUniform1f(GetUniformLocation(pName), pValue);
}

void Render::Render_Program::SetUniformVec2(const std::string& pName, const glm::vec2& pVec2)
{
	glUniform2f(GetUniformLocation(pName), pVec2.x, pVec2.y);
}

void Render::Render_Program::SetUniformVec3(const std::string& pName, const glm::vec3& pVec3)
{
	glUniform3f(GetUniformLocation(pName), pVec3.x, pVec3.y, pVec3.z);
}

void Render::Render_Program::SetUniformVec4(const std::string& pName, const glm::vec4& pVec4)
{
	glUniform4f(GetUniformLocation(pName), pVec4.x, pVec4.y, pVec4.z, pVec4.w);
}

void Render::Render_Program::SetUniformMat4(const std::string& pName, const glm::mat4& pMat4)
{
	glUniformMatrix4fv(GetUniformLocation(pName), 1, GL_TRUE, &pMat4[0][0]);
}

int Render::Render_Program::GetUniformInt(const std::string& pName)
{
	int value;
	glGetUniformiv(mId, GetUniformLocation(pName), &value);
	return value;
}

float Render::Render_Program::GetUniformFloat(const std::string& pName)
{
	float value;
	glGetUniformfv(mId, GetUniformLocation(pName), &value);
	return value;
}

glm::vec2 Render::Render_Program::GetUniformVec2(const std::string& pName)
{
	GLfloat values[2];
	glGetUniformfv(mId, GetUniformLocation(pName), values);
	return reinterpret_cast<glm::vec2&>(values);
}

glm::vec3 Render::Render_Program::GetUniformVec3(const std::string& pName)
{
	GLfloat values[3];
	glGetUniformfv(mId, GetUniformLocation(pName), values);
	return reinterpret_cast<glm::vec3&>(values);
}

glm::vec4 Render::Render_Program::GetUniformVec4(const std::string& pName)
{
	GLfloat values[4];
	glGetUniformfv(mId, GetUniformLocation(pName), values);
	return reinterpret_cast<glm::vec4&>(values);
}

glm::mat4 Render::Render_Program::GetUniformMat4(const std::string& pName)
{
	GLfloat values[16];
	glGetUniformfv(mId, GetUniformLocation(pName), values);
	return reinterpret_cast<glm::mat4&>(values);
}

const Render::Render_UniformInfo* Render::Render_Program::GetUniformInfo(const std::string& pName) const
{
	auto found = std::find_if(mUniforms.begin(), mUniforms.end(), [&pName](const Render_UniformInfo& pElement)
		{
			return pName == pElement.name;
		});

	if (found != mUniforms.end())
	{
		return &*found;
	}
	else
	{
		return nullptr;
	}
}

void Render::Render_Program::QueryUniforms()
{
	GLint numActiveUniforms = 0;
	mUniforms.clear();
	glGetProgramiv(mId, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
	std::vector<GLchar> nameData(256);
	for (int unif = 0; unif < numActiveUniforms; ++unif)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(mId, unif, static_cast<GLsizei>(nameData.size()), &actualLength, &arraySize, &type, &nameData[0]);
		std::string name(static_cast<char*>(nameData.data()), actualLength);

		if (!IsEngineUBOMember(name))
		{
			std::any defaultValue;

			switch (static_cast<EUniformType>(type))
			{
			case EUniformType::UNIFORM_BOOL:		defaultValue = std::make_any<bool>(GetUniformInt(name));			break;
			case EUniformType::UNIFORM_INT:			defaultValue = std::make_any<int>(GetUniformInt(name));				break;
			case EUniformType::UNIFORM_FLOAT:		defaultValue = std::make_any<float>(GetUniformFloat(name));			break;
			case EUniformType::UNIFORM_FLOAT_VEC2:	defaultValue = std::make_any<glm::vec2>(GetUniformVec2(name));		break;
			case EUniformType::UNIFORM_FLOAT_VEC3:	defaultValue = std::make_any<glm::vec3>(GetUniformVec3(name));		break;
			case EUniformType::UNIFORM_FLOAT_VEC4:	defaultValue = std::make_any<glm::vec4>(GetUniformVec4(name));		break;
			case EUniformType::UNIFORM_SAMPLER_2D:	defaultValue = std::make_any<Render_Texture*>(nullptr);	break;
			}

			if (defaultValue.has_value())
			{
				mUniforms.push_back
				({
					static_cast<EUniformType>(type),
					name,
					GetUniformLocation(nameData.data()),
					defaultValue
					});
			}
		}
	}
}

Render::Render_Program::Render_Program(const std::string pPath, uint32_t pId) :
	mPath(pPath), mId(pId)
{
	QueryUniforms();
}

Render::Render_Program::~Render_Program()
{
	glDeleteProgram(mId);
}

bool Render::Render_Program::IsEngineUBOMember(const std::string& pUniformName)
{
	return pUniformName.rfind("ubo_", 0) == 0;
}

uint32_t Render::Render_Program::GetUniformLocation(const std::string& pName)
{
	if (mUniformLocationCache.find(pName) != mUniformLocationCache.end())
	{
		return mUniformLocationCache.at(pName);
	}

	const int location = glGetUniformLocation(mId, pName.c_str());

	if (location == -1)
	{
		//OVLOG_WARNING("Uniform: '" + name + "' doesn't exist");
	}

	mUniformLocationCache[pName] = location;

	return location;
}
