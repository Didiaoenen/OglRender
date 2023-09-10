#include "Core_MaterialManager.h"

Core::Core_Material* Core::Core_MaterialManager::CreateResource(const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);

	Core::Core_Material* material = Core::Core_MaterialLoader::Create(realPath);
	if (material)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(material) + offsetof(Core::Core_Material, path)) = p_path;
	}

	return material;
}

void Core::Core_MaterialManager::DestroyResource(Core_Material* p_resource)
{
	Core::Core_MaterialLoader::Destroy(p_resource);
}

void Core::Core_MaterialManager::ReloadResource(Core_Material* p_resource, const std::string& p_path)
{
	Core::Core_MaterialLoader::Reload(*p_resource, p_path);
}
