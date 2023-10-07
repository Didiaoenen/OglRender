#include <regex>
#include <sstream>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

#include <OglCore/Core_ShaderManager.h>

#include "Render_Program.h"
#include "Render_ShaderLoader.h"

std::string Render::Render_ShaderLoader::__FILE_TRACE;

Render::Render_Shader* Render::Render_ShaderLoader::Create(const std::string& pFilePath)
{
	__FILE_TRACE = pFilePath;

	std::map<std::string, Render_Program*> programs = ParseShader(pFilePath);

	for (auto it = programs.begin(); it != programs.end(); it++)
	{
		auto& [_, program] = *it;

		program->mId = CreateProgram(program->mVertex, program->mFragment);
	}

	return new Render::Render_Shader(pFilePath, programs);
}

Render::Render_Shader* Render::Render_ShaderLoader::CreateFromSource(const std::string& pVertexShader, const std::string& pFragmentShader)
{
	std::map<std::string, Render_Program*> programs;

	Render_Program* program = new Render_Program();
	programs.emplace("universal", program);

	program->mName = "universal";
	program->mVertex = pVertexShader;
	program->mFragment = pFragmentShader;
	program->mId = CreateProgram(pVertexShader, pFragmentShader);

	return new Render_Shader("", programs);
}

void Render::Render_ShaderLoader::Recompile(Render_Shader& pShader, const std::string& pFilePath)
{
	__FILE_TRACE = pFilePath;

	std::map<std::string, Render_Program*> programs = ParseShader(pFilePath);

	for (auto it = programs.begin(); it != programs.end(); it++)
	{
		auto& [_, program] = *it;

		uint32_t newProgram = CreateProgram(program->mVertex, program->mFragment);

		if (newProgram)
		{
			std::uint32_t* programID = reinterpret_cast<uint32_t*>(&program) + offsetof(Render_Program, mId);

			glDeleteProgram(*programID);

			*programID = newProgram;

			pShader.QueryUniforms();

			//OVLOG_INFO("[COMPILE] \"" + __FILE_TRACE + "\": Success!");
		}
		else
		{
			//OVLOG_ERROR("[COMPILE] \"" + __FILE_TRACE + "\": Failed! Previous shader version kept");
		}
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

std::map<std::string, Render::Render_Program*> Render::Render_ShaderLoader::ParseShader(const std::string& pFilePath)
{
	std::string line, name;

	std::map<std::string, Render_Program*> programs;

	std::string states[] = { "#blend", "#culling", "#depth_test", "#depth_writing", "#color_writing" };

	std::ifstream stream(pFilePath);
	while (std::getline(stream, line))
	{
		if (line.find("#pass") != std::string::npos)
		{
			std::smatch sm;
			std::regex_search(line, sm, std::regex("(\\w+)[ ](\\w+)"));

			Render_Program *program = new Render_Program();
			program->mName = name = sm[2];

			programs.emplace(name, program);
		}
		
		for (auto& state : states)
		{
			if (line.find(state) != std::string::npos)
			{
				if (auto it = programs.find(name); it != programs.end())
				{
					auto& [_, program] = *it;

					std::smatch sm;
					std::regex_search(line, sm, std::regex("(\\w+)[ ](\\w+)"));
					program->mPipelineState.emplace(sm[1].str(), std::stoi(sm[2]));
				}
			}
		}

		if (line.find("#include") != std::string::npos)
		{
			if (auto it = programs.find(name); it != programs.end())
			{
				auto& [_, program] = *it;

				std::smatch sm;
				std::regex_search(line, sm, std::regex("(\").*(\")"));
				program->mPath = std::regex_replace(sm[0].str(), std::regex("(\")"), "");
			}
		}
	}

	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

	for (auto it = programs.begin(); it != programs.end(); it++)
	{
		auto& [name, program] = *it;

		std::ifstream stream(Core::Core_ShaderManager::GetRealPath(program->mPath));

		ShaderType type = ShaderType::NONE;

		std::stringstream ss[2];

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

		program->mVertex = ss[static_cast<int>(ShaderType::VERTEX)].str();
		program->mFragment = ss[static_cast<int>(ShaderType::FRAGMENT)].str();
	}

	return programs;
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

		std::cout << errorHeader + __FILE_TRACE + "\":\n" + errorLog << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}
