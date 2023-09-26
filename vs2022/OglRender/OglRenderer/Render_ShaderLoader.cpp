#include <regex>
#include <sstream>
#include <fstream>

#include <GL/glew.h>

#include "Render_ShaderLoader.h"

std::string Render::Render_ShaderLoader::__FILE_TRACE;

Render::Render_Shader* Render::Render_ShaderLoader::Create(const std::string& pFilePath)
{
	__FILE_TRACE = pFilePath;

	std::pair<std::string, std::string> source = ParseShader(pFilePath);

	auto a = ParseShader2(pFilePath);

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

/*
* #name default
* #state_begine
* #blend_src 0
* #blend_dst 0
* #depth_test 0
* #depth_write 0
* #color_mask 0
* #cull 0
* #state_end
* 
* #shader vertex
*
* #shader fragment
* 
* #shader geometry
*/

std::pair<std::string, std::string> Render::Render_ShaderLoader::ParseShader(const std::string& pFilePath)
{
	std::ifstream stream(pFilePath);

	std::string line;

	std::stringstream ss[10];

	std::regex e("(\\w+)[ ](\\w+)");

	EShaderType type = EShaderType::NONE;
	while (std::getline(stream, line))
	{
		if (line.find("#name") != std::string::npos)
		{
			type = EShaderType::NONE;

			std::smatch sm;
			std::regex_search(line, sm, e);
			std::string name = sm[2];
		}
		
		bool state = false;
		if (line.find("#state_begine") != std::string::npos)
		{
			state = true;
		}
		else if (line.find("#state_end") != std::string::npos)
		{
			state = false;
		}

		if (state)
		{

		}
		else
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = EShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = EShaderType::FRAGMENT;
				}
				else if (line.find("geometry") != std::string::npos)
				{
					type = EShaderType::GEOMETRY;
				}
			}
			else if (type != EShaderType::NONE)
			{
				ss[static_cast<int>(type)] << line << '\n';
			}
		}
	}

	return
	{
		ss[static_cast<int>(EShaderType::VERTEX)].str(),
		ss[static_cast<int>(EShaderType::FRAGMENT)].str(),
	};
}

std::map<std::string, Render::Render_ParseData*> Render::Render_ShaderLoader::ParseShader2(const std::string& pFilePath)
{
	std::ifstream stream(pFilePath);

	std::string name;

	std::string line;

	bool state = false;

	std::regex e("(\\w+)[ ](\\w+)");

	EShaderType type = EShaderType::NONE;

	std::map<std::string, Render_ParseData*> datas;

	std::map<std::string, std::map<int, std::stringstream>> sss;

	while (std::getline(stream, line))
	{
		if (line.find("#name") != std::string::npos)
		{
			Render_ParseData* data = new Render_ParseData();

			type = EShaderType::NONE;

			std::smatch sm;
			std::regex_search(line, sm, e);
			name = data->name = sm[2];

			datas.emplace(data->name, data);
		}else if (line.find("#state_begine") != std::string::npos)
		{
			state = true;
		}
		else if (line.find("#state_end") != std::string::npos)
		{
			state = false;
		}
		else
		{
			if (state)
			{
				std::smatch sm;
				std::regex_search(line, sm, e);
				datas[name]->state.emplace(sm[1], std::stoi(sm[2]));
			}
			else
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
					{
						type = EShaderType::VERTEX;
					}
					else if (line.find("fragment") != std::string::npos)
					{
						type = EShaderType::FRAGMENT;
					}
					else if (line.find("geometry") != std::string::npos)
					{
						type = EShaderType::GEOMETRY;
					}

					if (auto temp = sss.find(name); temp == sss.end())
					{
						sss.emplace(name, std::map<int, std::stringstream>());
					}
					sss[name].emplace(static_cast<int>(type), std::stringstream());
				}
				else if (type != EShaderType::NONE)
				{
					sss[name][static_cast<int>(type)] << line << '\n';
				}
			}
		}
	}

	for (auto it = datas.begin(); it != datas.end(); it++)
	{
		auto& [name, data] = *it;
		data->vertex = sss[name][static_cast<int>(EShaderType::VERTEX)].str();
		data->fragment = sss[name][static_cast<int>(EShaderType::FRAGMENT)].str();
		data->geometry = sss[name][static_cast<int>(EShaderType::GEOMETRY)].str();
	}

	return datas;
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
