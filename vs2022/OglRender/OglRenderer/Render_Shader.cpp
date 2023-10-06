
#include <GL/glew.h>

#include "Render_Texture.h"
#include "Render_Shader.h"

const Render::Render_UniformInfo* Render::Render_Shader::GetUniformInfo(const std::string& pName) const
{
	if (auto it = mUniforms.find(pName); it != mUniforms.end())
	{
		auto& [_, uniform] = *it;

		return &uniform;
	}
	return nullptr;
}

Render::Render_Program* Render::Render_Shader::GetRenderProgram(const std::string& pName)
{
	if (auto it = mPrograms.find(pName); it != mPrograms.end())
	{
		auto& [_, program] = *it;

		return program;
	}
	return nullptr;
}

void Render::Render_Shader::QueryUniforms()
{
	GLint numActiveUniforms = 0;
	mUniforms.clear();

	for (auto it = mPrograms.begin(); it != mPrograms.end(); it++)
	{
		auto& [_, program] = *it;

		glGetProgramiv(program->mId, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
		std::vector<GLchar> nameData(256);
		for (int unif = 0; unif < numActiveUniforms; ++unif)
		{
			GLint arraySize = 0;
			GLenum type = 0;
			GLsizei actualLength = 0;
			glGetActiveUniform(program->mId, unif, static_cast<GLsizei>(nameData.size()), &actualLength, &arraySize, &type, &nameData[0]);
			std::string name(static_cast<char*>(nameData.data()), actualLength);

			if (!IsEngineUBOMember(name))
			{
				std::any defaultValue;

				EUniformType eType = static_cast<EUniformType>(type);

				switch (eType)
				{
				case EUniformType::UNIFORM_BOOL:		defaultValue = std::make_any<bool>(program->GetUniformInt(name));			break;
				case EUniformType::UNIFORM_INT:			defaultValue = std::make_any<int>(program->GetUniformInt(name));			break;
				case EUniformType::UNIFORM_FLOAT:		defaultValue = std::make_any<float>(program->GetUniformFloat(name));		break;
				case EUniformType::UNIFORM_FLOAT_VEC2:	defaultValue = std::make_any<glm::vec2>(program->GetUniformVec2(name));		break;
				case EUniformType::UNIFORM_FLOAT_VEC3:	defaultValue = std::make_any<glm::vec3>(program->GetUniformVec3(name));		break;
				case EUniformType::UNIFORM_FLOAT_VEC4:	defaultValue = std::make_any<glm::vec4>(program->GetUniformVec4(name));		break;
				case EUniformType::UNIFORM_SAMPLER_2D:	defaultValue = std::make_any<Render_Texture*>(nullptr);	break;
				}

				if (defaultValue.has_value())
				{
					Render_UniformInfo info = 
					{
						eType,
						name,
						GetUniformLocation(program, nameData.data()),
						defaultValue
					};

					if (auto it = mUniforms.find(name); it != mUniforms.end())
					{
						auto& [_, uniform] = *it;
						if (eType != uniform.type)
						{
							// TODO
						}
					}
					mUniforms.emplace(name, info);
				}
			}
		}
	}

}

Render::Render_Shader::Render_Shader(const std::string pPath, std::map<std::string, Render_Program*> pPrograms) :
	mPath(pPath), mPrograms(pPrograms)
{
	QueryUniforms();
}

Render::Render_Shader::~Render_Shader()
{
}

bool Render::Render_Shader::IsEngineUBOMember(const std::string& pUniformName)
{
	return pUniformName.rfind("ubo_", 0) == 0;
}

uint32_t Render::Render_Shader::GetUniformLocation(const Render_Program* pProgram, const std::string& pName)
{
	if (mUniformLocationCache.find(pName) != mUniformLocationCache.end())
	{
		return mUniformLocationCache.at(pName);
	}

	const int location = glGetUniformLocation(pProgram->mId, pName.c_str());

	if (location == -1)
	{
		//OVLOG_WARNING("Uniform: '" + name + "' doesn't exist");
	}

	mUniformLocationCache[pName] = location;

	return location;
}
