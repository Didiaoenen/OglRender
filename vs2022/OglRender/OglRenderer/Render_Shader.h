#pragma once

#include <map>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Render_Program.h"
#include "Render_UniformInfo.h"

namespace Render
{
	class Render_ShaderLoader;

	class Render_Shader
	{
		friend class Render_ShaderLoader;

	public:
		const Render_UniformInfo* GetUniformInfo(const std::string& pName) const;

		Render_Program* GetRenderProgram(const std::string& pName);

		void QueryUniforms();

	private:
		Render_Shader(const std::string pPath, std::map<std::string, Render_Program*> pPrograms);
		~Render_Shader();

		static bool IsEngineUBOMember(const std::string& pUniformName);
		uint32_t GetUniformLocation(const Render_Program* pProgram, const std::string& pName);

	public:
		const std::string mPath;
		std::map<std::string, Render_Program*> mPrograms;
		std::map<std::string, Render_UniformInfo> mUniforms;

	private:
		std::unordered_map<std::string, int> mUniformLocationCache;
	};
}

