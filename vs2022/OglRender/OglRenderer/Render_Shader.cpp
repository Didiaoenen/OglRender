
#include <GL/glew.h>

#include "Render_Texture.h"
#include "Render_Shader.h"

void Render::Render_Shader::QueryUniforms()
{
	for (auto it = mPrograms.begin(); it != mPrograms.end(); it++)
	{
		auto& [name, program] = *it;

		mUniforms.clear();
		GLint numActiveUniforms = 0;
		std::vector<GLchar> nameData(256);
		glGetProgramiv(program.mId, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
		for (int unif = 0; unif < numActiveUniforms; ++unif)
		{
			GLint size = 0;
			GLenum type = 0;
			GLsizei actualLength = 0;
			glGetActiveUniform(program.mId, unif, static_cast<GLsizei>(nameData.size()), &actualLength, &size, &type, &nameData[0]);
			std::string uniformName(static_cast<char*>(nameData.data()), actualLength);

			if (!IsEngineUBOMember(name))
			{
				std::any defaultValue;

				switch (static_cast<EUniformType>(type))
				{
				case EUniformType::UNIFORM_BOOL:		defaultValue = std::make_any<bool>(program.GetUniformInt(uniformName));			break;
				case EUniformType::UNIFORM_INT:			defaultValue = std::make_any<int>(program.GetUniformInt(uniformName));			break;
				case EUniformType::UNIFORM_FLOAT:		defaultValue = std::make_any<float>(program.GetUniformFloat(uniformName));		break;
				case EUniformType::UNIFORM_FLOAT_VEC2:	defaultValue = std::make_any<glm::vec2>(program.GetUniformVec2(uniformName));	break;
				case EUniformType::UNIFORM_FLOAT_VEC3:	defaultValue = std::make_any<glm::vec3>(program.GetUniformVec3(uniformName));	break;
				case EUniformType::UNIFORM_FLOAT_VEC4:	defaultValue = std::make_any<glm::vec4>(program.GetUniformVec4(uniformName));	break;
				case EUniformType::UNIFORM_SAMPLER_2D:	defaultValue = std::make_any<Render_Texture*>(nullptr);	break;
				}

				if (defaultValue.has_value())
				{
					if (auto it = mUniforms.find(uniformName); it != mUniforms.end())
					{
						auto& [_, uniformInfo] = *it;

						auto eType = static_cast<EUniformType>(type);
						if (eType != uniformInfo.type)
						{

						}
						else
						{
							Render_UniformInfo info = {
								eType,
								uniformName,
								GetUniformLocation(name, nameData.data()),
								defaultValue
							};
							mUniforms.emplace(uniformName, info);
						}
					}
				}
			}
		}
	}
}

const Render::Render_UniformInfo* Render::Render_Shader::GetUniformInfo(const std::string& pName) const
{
	auto it = mUniforms.find(pName);
	if (it != mUniforms.end())
	{
		auto& [_, uniformInfo] = *it;

		return &uniformInfo;
	}
		
	return nullptr;
}

Render::Render_Shader::Render_Shader(const std::string& pPath, const std::map<std::string, Render_Program> pPrograms) :
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

uint32_t Render::Render_Shader::GetUniformLocation(const std::string& pName, const std::string& pUniformName)
{
	if (auto it = mPrograms.find(pName); it != mPrograms.end())
	{
		auto& [_, program] = *it;

		if (mUniformLocationCache.find(pUniformName) != mUniformLocationCache.end())
		{
			return mUniformLocationCache.at(pUniformName);
		}

		const int location = glGetUniformLocation(program.mId, pUniformName.c_str());

		if (location == -1)
		{
			//OVLOG_WARNING("Uniform: '" + name + "' doesn't exist");
		}

		mUniformLocationCache[pUniformName] = location;

		return location;
	}

	return 0;
}