#pragma once

#include "Render_Shader.h"

namespace Render
{
	class Render_ShaderLoader
	{
	public:
		Render_ShaderLoader() = delete;

		static Render_Shader* Create(const std::string& pFilePath);

		static Render_Shader* CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader);

		static void	Recompile(Render_Shader& p_shader, const std::string& pFilePath);

		static bool Destroy(Render_Shader*& p_shader);

	private:
		static std::pair<std::string, std::string> ParseShader(const std::string& pFilePath);
		static uint32_t CreateProgram(const std::string& p_vertexShader, const std::string& p_fragmentShader);
		static uint32_t CompileShader(uint32_t p_type, const std::string& pSource);

		static std::string __FILE_TRACE;
	};
}

