#pragma once

#include <string>
#include <vector>

#include "Render_Texture.h"

namespace Render
{
	class Render_TextureLoader
	{
	public:
		Render_TextureLoader() = delete;

		static Render_Texture* Create(const std::string& p_filepath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);

		static Render_Texture* CreateColor(uint32_t p_data, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);

		static Render_Texture* CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);

		static void Reload(Render_Texture& p_texture, const std::string& p_filePath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);

		static bool Destroy(Render_Texture*& p_textureInstance);
	};
}

