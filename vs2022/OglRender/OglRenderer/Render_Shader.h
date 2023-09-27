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
		void QueryUniforms();

		const Render_UniformInfo* GetUniformInfo(const std::string& pName) const;
	private:
		Render_Shader(const std::string& pPath, const std::map<std::string, Render_Program> pPrograms);
		~Render_Shader();

		static bool IsEngineUBOMember(const std::string& pUniformName);
		uint32_t GetUniformLocation(const std::string& pName, const std::string& pUniformName);

	public:
		const std::string mPath;
		std::map<std::string, Render_Program> mPrograms;
		std::map<std::string, Render_UniformInfo> mUniforms;

	private:
		std::unordered_map<std::string, int> mUniformLocationCache;
	};
}

