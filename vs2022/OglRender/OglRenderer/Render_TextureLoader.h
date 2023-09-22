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

		static Render_Texture* Create(const std::string& p_filepath, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap);

		static Render_Texture* CreateColor(uint32_t pData, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap);

		static Render_Texture* CreateFromMemory(uint8_t* pData, uint32_t pWidth, uint32_t pHeight, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap);

		static void Reload(Render_Texture& p_texture, const std::string& pFilePath, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap);

		static bool Destroy(Render_Texture*& p_textureInstance);
	};
}

