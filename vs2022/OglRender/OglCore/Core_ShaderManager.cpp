#include "Core_ShaderManager.h"

Render::Render_Shader* Core::Core_ShaderManager::CreateResource(const std::string& pPath)
{
	std::string realPath = GetRealPath(pPath);
	Render::Render_Shader* shader = Render::Render_ShaderLoader::Create(realPath, Core_ShaderManager::GetAssetPaths().first);
	if (shader)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(shader) + offsetof(Render::Render_Shader, mPath)) = pPath;
	}

	return shader;
}

void Core::Core_ShaderManager::DestroyResource(Render::Render_Shader* pResource)
{
	Render::Render_ShaderLoader::Destroy(pResource);
}

void Core::Core_ShaderManager::ReloadResource(Render::Render_Shader* pResource, const std::string& pPath)
{
	Render::Render_ShaderLoader::Recompile(*pResource, pPath);
}
