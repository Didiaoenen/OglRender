
#include <GL/glew.h>

#include "Render_Texture.h"

void Render::Render_Texture::Bind(uint32_t p_slot) const
{
}

void Render::Render_Texture::Unbind() const
{
}

Render::Render_Texture::Render_Texture(const std::string pPath, uint32_t pId, uint32_t p_width, uint32_t p_height, uint32_t p_bpp, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap) :
	id(pId), width(p_width), height(p_height), bitsPerPixel(p_bpp), firstFilter(p_firstFilter), secondFilter(p_secondFilter), isMimapped(p_generateMipmap)
{
}
