
#include <GL/glew.h>

#include "Render_TextureLoader.h"

Render::Render_Texture* Render::Render_TextureLoader::Create(const std::string& p_filepath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
    return nullptr;
}

Render::Render_Texture* Render::Render_TextureLoader::CreateColor(uint32_t p_data, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
    return nullptr;
}

Render::Render_Texture* Render::Render_TextureLoader::CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
    return nullptr;
}

void Render::Render_TextureLoader::Reload(Render_Texture& p_texture, const std::string& pFilePath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
}

bool Render::Render_TextureLoader::Destroy(Render_Texture*& p_textureInstance)
{
    return false;
}
