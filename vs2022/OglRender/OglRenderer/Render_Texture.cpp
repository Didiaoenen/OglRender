
#include <GL/glew.h>

#include "Render_Texture.h"

void Render::Render_Texture::Bind(uint32_t pSlot) const
{
	glActiveTexture(GL_TEXTURE0 + pSlot);
	glBindTexture(GL_TEXTURE_2D, mId);
}

void Render::Render_Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Render::Render_Texture::Render_Texture(const std::string pPath, uint32_t pId, uint32_t pWidth, uint32_t pHeight, uint32_t pBpp, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap) :
	mId(pId), mWidth(pWidth), mHeight(pHeight), mBitsPerPixel(pBpp), mFirstFilter(pFirstFilter), mSecondFilter(pSecondFilter), mIsMimapped(pGenerateMipmap)
{
}
