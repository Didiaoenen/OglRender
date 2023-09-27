#pragma once

#include <map>
#include <string>

#include "Render_EShaderType.h"

#include "Render_Shader.h"

namespace Render
{
	struct Render_ParseData
	{
		std::string name;
		std::string vertex;
		std::string fragment;
		std::string geometry;
		std::map<std::string, int> state;
	};

	struct Render_ShaderBuffer
	{
		std::string vertex;
		std::string fragment;
		std::string geometry;
	};

	class Render_ShaderLoader
	{
	public:
		Render_ShaderLoader() = delete;

		static Render_Shader* Create(const std::string& pFilePath);

		static Render_Program* CreateFromSource(const std::string& pVertexShader, const std::string& pFragmentShader);

		static void	Recompile(Render_Shader& pShader, const std::string& pFilePath);

		static bool Destroy(Render_Shader*& pShader);

	private:
		static std::map<std::string, Render_ParseData> ParseShader(const std::string& pFilePath);

		static uint32_t CreateProgram(const Render_ShaderBuffer& buffer);
		static uint32_t CompileShader(uint32_t pType, const std::string& pSource);

		static std::string __FILE_TRACE;
	};
}

