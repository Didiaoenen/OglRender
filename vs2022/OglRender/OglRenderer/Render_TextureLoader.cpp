
#include <GL/glew.h>
#include <stb_image.h>

#include "Render_TextureLoader.h"

Render::Render_Texture* Render::Render_TextureLoader::Create(const std::string& p_filepath, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap)
{
	GLuint textureID;
	int textureWidth;
	int textureHeight;
	int bitsPerPixel;
	glGenTextures(1, &textureID);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* dataBuffer = stbi_load(p_filepath.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);

	if (dataBuffer)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);

		if (pGenerateMipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(pFirstFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(pSecondFilter));

		stbi_image_free(dataBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		return new Render_Texture(p_filepath, textureID, textureWidth, textureHeight, bitsPerPixel, pFirstFilter, pSecondFilter, pGenerateMipmap);
	}
	else
	{
		stbi_image_free(dataBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		return nullptr;
	}
}

Render::Render_Texture* Render::Render_TextureLoader::CreateColor(uint32_t pData, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pData);

	if (pGenerateMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(pFirstFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(pSecondFilter));

	glBindTexture(GL_TEXTURE_2D, 0);

	return new Render_Texture("", textureID, 1, 1, 32, pFirstFilter, pSecondFilter, pGenerateMipmap);
}

Render::Render_Texture* Render::Render_TextureLoader::CreateFromMemory(uint8_t* pData, uint32_t pWidth, uint32_t pHeight, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pWidth, pHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);

	if (pGenerateMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(pFirstFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(pSecondFilter));

	glBindTexture(GL_TEXTURE_2D, 0);

	return new Render_Texture("", textureID, 1, 1, 32, pFirstFilter, pSecondFilter, pGenerateMipmap);
}

void Render::Render_TextureLoader::Reload(Render_Texture& p_texture, const std::string& pFilePath, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap)
{
	Render_Texture* newTexture = Create(pFilePath, pFirstFilter, pSecondFilter, pGenerateMipmap);

	if (newTexture)
	{
		glDeleteTextures(1, &p_texture.mId);

		*const_cast<uint32_t*>(&p_texture.mId) = newTexture->mId;
		*const_cast<uint32_t*>(&p_texture.mWidth) = newTexture->mWidth;
		*const_cast<uint32_t*>(&p_texture.mHeight) = newTexture->mHeight;
		*const_cast<uint32_t*>(&p_texture.mBitsPerPixel) = newTexture->mBitsPerPixel;
		*const_cast<ETextureFilteringMode*>(&p_texture.mFirstFilter) = newTexture->mFirstFilter;
		*const_cast<ETextureFilteringMode*>(&p_texture.mSecondFilter) = newTexture->mSecondFilter;
		*const_cast<bool*>(&p_texture.mIsMimapped) = newTexture->mIsMimapped;
		delete newTexture;
	}
}

bool Render::Render_TextureLoader::Destroy(Render_Texture*& p_textureInstance)
{
	if (p_textureInstance)
	{
		glDeleteTextures(1, &p_textureInstance->mId);
		delete p_textureInstance;
		p_textureInstance = nullptr;
		return true;
	}

	return false;
}
