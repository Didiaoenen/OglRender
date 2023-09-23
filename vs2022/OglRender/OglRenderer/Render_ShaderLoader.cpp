#include <sstream>
#include <fstream>

#include <GL/glew.h>

#include "Render_ShaderLoader.h"

std::string Render::Render_ShaderLoader::__FILE_TRACE;

Render::Render_Shader* Render::Render_ShaderLoader::Create(const std::string& pFilePath)
{
	__FILE_TRACE = pFilePath;

	std::pair<std::string, std::string> source = ParseShader(pFilePath);

	uint32_t programID = CreateProgram(source.first, source.second);

	if (programID)
	{
		return new Render_Shader(pFilePath, programID);
	}

	return nullptr;
}

Render::Render_Shader* Render::Render_ShaderLoader::CreateFromSource(const std::string& pVertexShader, const std::string& pFragmentShader)
{
	uint32_t programID = CreateProgram(pVertexShader, pFragmentShader);

	if (programID)
	{
		return new Render_Shader("", programID);
	}

	return nullptr;
}

void Render::Render_ShaderLoader::Recompile(Render_Shader& pShader, const std::string& pFilePath)
{
	__FILE_TRACE = pFilePath;

	std::pair<std::string, std::string> source = ParseShader(pFilePath);

	uint32_t newProgram = CreateProgram(source.first, source.second);

	if (newProgram)
	{
		std::uint32_t* shaderID = reinterpret_cast<uint32_t*>(&pShader) + offsetof(Render_Shader, mId);

		glDeleteProgram(*shaderID);

		*shaderID = newProgram;

		pShader.QueryUniforms();

		//OVLOG_INFO("[COMPILE] \"" + __FILE_TRACE + "\": Success!");
	}
	else
	{
		//OVLOG_ERROR("[COMPILE] \"" + __FILE_TRACE + "\": Failed! Previous shader version kept");
	}
}

bool Render::Render_ShaderLoader::Destroy(Render_Shader*& pShader)
{
	if (pShader)
	{
		delete pShader;
		pShader = nullptr;

		return true;
	}

	return false;
}

std::pair<std::string, std::string> Render::Render_ShaderLoader::ParseShader(const std::string& pFilePath)
{
	std::ifstream stream(pFilePath);

	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

	std::string line;

	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else if (type != ShaderType::NONE)
		{
			ss[static_cast<int>(type)] << line << '\n';
		}
	}

	return
	{
		ss[static_cast<int>(ShaderType::VERTEX)].str(),
		ss[static_cast<int>(ShaderType::FRAGMENT)].str()
	};
}

uint32_t Render::Render_ShaderLoader::CreateProgram(const std::string& pVertexShader, const std::string& pFragmentShader)
{
	const uint32_t program = glCreateProgram();

	const uint32_t vs = CompileShader(GL_VERTEX_SHADER, pVertexShader);
	const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, pFragmentShader);

	if (vs == 0 || fs == 0)
	{
		return 0;
	}

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());

		//OVLOG_ERROR("[LINK] \"" + __FILE_TRACE + "\":\n" + errorLog);

		glDeleteProgram(program);

		return 0;
	}

	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

uint32_t Render::Render_ShaderLoader::CompileShader(uint32_t pType, const std::string& pSource)
{
	const uint32_t id = glCreateShader(pType);

	const char* src = pSource.c_str();

	glShaderSource(id, 1, &src, nullptr);

	glCompileShader(id);

	GLint compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetShaderInfoLog(id, maxLength, &maxLength, errorLog.data());

		std::string shaderTypeString = pType == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER";
		std::string errorHeader = "[" + shaderTypeString + "] \"";
		//OVLOG_ERROR(errorHeader + __FILE_TRACE + "\":\n" + errorLog);

		glDeleteShader(id);

		return 0;
	}

	return id;
}
