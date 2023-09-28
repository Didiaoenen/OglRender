#pragma once

#include "Render_Shader.h"

namespace Render
{
	class Render_ShaderLoader
	{
	public:
		Render_ShaderLoader() = delete;

		static Render_Shader* Create(const std::string& pFilePath);

		static Render_Shader* CreateFromSource(const std::string& pVertexShader, const std::string& pFragmentShader);

		static void	Recompile(Render_Shader& pShader, const std::string& pFilePath);

		static bool Destroy(Render_Shader*& pShader);

	private:
		static std::pair<std::string, std::string> ParseShader(const std::string& pFilePath);
		static uint32_t CreateProgram(const std::string& pVertexShader, const std::string& pFragmentShader);
		static uint32_t CompileShader(uint32_t pType, const std::string& pSource);

		static std::string __FILE_TRACE;
	};
}

