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
		void Bind(uint32_t pSlot = 0) const;

		void Unbind() const;

	private:
		Render_Texture(const std::string pPath, uint32_t pId, uint32_t pWidth, uint32_t pHeight, uint32_t pBpp, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap);
		~Render_Texture() = default;

	public:
		const uint32_t mId;
		const uint32_t mWidth;
		const uint32_t mHeight;
		const uint32_t mBitsPerPixel;
		const ETextureFilteringMode mFirstFilter;
		const ETextureFilteringMode mSecondFilter;
		const std::string mPath;
		const bool mIsMimapped;
	};
}

