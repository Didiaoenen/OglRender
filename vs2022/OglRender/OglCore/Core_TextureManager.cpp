
#include <OglTools/Tools_IniFile.h>

#include <OglRenderer/Render_DriverSettings.h>

#include "Core_TextureManager.h"

std::tuple<Render::ETextureFilteringMode, Render::ETextureFilteringMode, bool> GetAssetMetadata(const std::string& pPath)
{
	auto metaFile = Tools::Tools_IniFile(pPath + ".meta");

	auto min = metaFile.GetOrDefault("MIN_FILTER", static_cast<int>(Render::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	auto mag = metaFile.GetOrDefault("MAG_FILTER", static_cast<int>(Render::ETextureFilteringMode::LINEAR));
	auto mipmap = metaFile.GetOrDefault("ENABLE_MIPMAPPING", true);

	return { static_cast<Render::ETextureFilteringMode>(min), static_cast<Render::ETextureFilteringMode>(mag), mipmap };
}

Render::Render_Texture* Core::Core_TextureManager::CreateResource(const std::string& pPath)
{
	std::string realPath = GetRealPath(pPath);

	auto [min, mag, mipmap] = GetAssetMetadata(realPath);

	Render::Render_Texture* texture = Render::Render_TextureLoader::Create(realPath, min, mag, mipmap);
	if (texture)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(texture) + offsetof(Render::Render_Texture, path)) = pPath;

	return texture;
}

void Core::Core_TextureManager::DestroyResource(Render::Render_Texture* p_resource)
{
	Render::Render_TextureLoader::Destroy(p_resource);
}

void Core::Core_TextureManager::ReloadResource(Render::Render_Texture* p_resource, const std::string& pPath)
{
	std::string realPath = GetRealPath(pPath);

	auto [min, mag, mipmap] = GetAssetMetadata(realPath);

	Render::Render_TextureLoader::Reload(*p_resource, realPath, min, mag, mipmap);
}
