#pragma once

#include <stdint.h>
#include <string>

#include "Render_ETextureFilteringMode.h"

namespace Render
{
	class Render_TextureLoader;

	class Render_Texture
	{
		friend class Render_TextureLoader;

	public:
		void Bind(uint32_t p_slot = 0) const;

		void Unbind() const;

	private:
		Render_Texture(const std::string p_path, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bpp, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
		~Render_Texture() = default;

	public:
		const uint32_t id;
		const uint32_t width;
		const uint32_t height;
		const uint32_t bitsPerPixel;
		const ETextureFilteringMode firstFilter;
		const ETextureFilteringMode secondFilter;
		const std::string path;
		const bool isMimapped;
	};
}

