
#include <GL/glew.h>
#include <stb_image.h>

#include "Render_TextureLoader.h"

Render::Render_Texture* Render::Render_TextureLoader::Create(const std::string& pFilepath, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap)
{
	GLuint textureID;
	int textureWidth;
	int textureHeight;
	int bitsPerPixel;
	glGenTextures(1, &textureID);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* dataBuffer = stbi_load(pFilepath.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);

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

		return new Render_Texture(pFilepath, textureID, textureWidth, textureHeight, bitsPerPixel, pFirstFilter, pSecondFilter, pGenerateMipmap);
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

void Render::Render_TextureLoader::Reload(Render_Texture& pTexture, const std::string& pFilePath, ETextureFilteringMode pFirstFilter, ETextureFilteringMode pSecondFilter, bool pGenerateMipmap)
{
	Render_Texture* newTexture = Create(pFilePath, pFirstFilter, pSecondFilter, pGenerateMipmap);

	if (newTexture)
	{
		glDeleteTextures(1, &pTexture.mId);

		*const_cast<uint32_t*>(&pTexture.mId) = newTexture->mId;
		*const_cast<uint32_t*>(&pTexture.mWidth) = newTexture->mWidth;
		*const_cast<uint32_t*>(&pTexture.mHeight) = newTexture->mHeight;
		*const_cast<uint32_t*>(&pTexture.mBitsPerPixel) = newTexture->mBitsPerPixel;
		*const_cast<ETextureFilteringMode*>(&pTexture.mFirstFilter) = newTexture->mFirstFilter;
		*const_cast<ETextureFilteringMode*>(&pTexture.mSecondFilter) = newTexture->mSecondFilter;
		*const_cast<bool*>(&pTexture.mIsMimapped) = newTexture->mIsMimapped;
		delete newTexture;
	}
}

bool Render::Render_TextureLoader::Destroy(Render_Texture*& pTextureInstance)
{
	if (pTextureInstance)
	{
		glDeleteTextures(1, &pTextureInstance->mId);
		delete pTextureInstance;
		pTextureInstance = nullptr;
		return true;
	}

	return false;
}
