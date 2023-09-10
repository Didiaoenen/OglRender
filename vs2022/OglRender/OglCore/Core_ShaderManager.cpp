#include "Core_ShaderManager.h"

Render::Render_Shader* Core::Core_ShaderManager::CreateResource(const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);
	Render::Render_Shader* shader = Render::Render_ShaderLoader::Create(realPath);
	if (shader)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(shader) + offsetof(Render::Render_Shader, path)) = p_path;

	return shader;
}

void Core::Core_ShaderManager::DestroyResource(Render::Render_Shader* p_resource)
{
	Render::Render_ShaderLoader::Destroy(p_resource);
}

void Core::Core_ShaderManager::ReloadResource(Render::Render_Shader* p_resource, const std::string& p_path)
{
	Render::Render_ShaderLoader::Recompile(*p_resource, p_path);
}
