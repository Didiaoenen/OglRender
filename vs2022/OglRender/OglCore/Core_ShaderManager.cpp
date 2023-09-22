#include "Core_ShaderManager.h"

Render::Render_Shader* Core::Core_ShaderManager::CreateResource(const std::string& pPath)
{
	std::string realPath = GetRealPath(pPath);
	Render::Render_Shader* shader = Render::Render_ShaderLoader::Create(realPath);
	if (shader)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(shader) + offsetof(Render::Render_Shader, mPath)) = pPath;
	}

	return shader;
}

void Core::Core_ShaderManager::DestroyResource(Render::Render_Shader* p_resource)
{
	Render::Render_ShaderLoader::Destroy(p_resource);
}

void Core::Core_ShaderManager::ReloadResource(Render::Render_Shader* p_resource, const std::string& pPath)
{
	Render::Render_ShaderLoader::Recompile(*p_resource, pPath);
}
