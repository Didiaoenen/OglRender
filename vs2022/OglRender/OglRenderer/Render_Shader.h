#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "Render_UniformInfo.h"

namespace Render
{
	class Render_ShaderLoader;

	class Render_Shader
	{
		friend class Render_ShaderLoader;

	public:
		void Bind() const;

		void Unbind() const;

		void SetUniformInt(const std::string& pName, int pValue);

		void SetUniformFloat(const std::string& pName, float pValue);

		void SetUniformVec2(const std::string& pName, const glm::vec2& pVec2);

		void SetUniformVec3(const std::string& pName, const glm::vec3& pVec3);

		void SetUniformVec4(const std::string& pName, const glm::vec4& pVec4);

		void SetUniformMat4(const std::string& pName, const glm::mat4& pMat4);

		int GetUniformInt(const std::string& pName);

		float GetUniformFloat(const std::string& pName);

		glm::vec2 GetUniformVec2(const std::string& pName);

		glm::vec3 GetUniformVec3(const std::string& pName);

		glm::vec4 GetUniformVec4(const std::string& pName);

		glm::mat4 GetUniformMat4(const std::string& pName);

		const Render_UniformInfo* GetUniformInfo(const std::string& pName) const;

		void QueryUniforms();

	private:
		Render_Shader(const std::string pPath, uint32_t pId);
		~Render_Shader();

		static bool IsEngineUBOMember(const std::string& pUniformName);
		uint32_t GetUniformLocation(const std::string& pName);

	public:
		const uint32_t mId;
		const std::string mPath;
		std::vector<Render_UniformInfo> mUniforms;

	private:
		std::unordered_map<std::string, int> mUniformLocationCache;
	};
}

