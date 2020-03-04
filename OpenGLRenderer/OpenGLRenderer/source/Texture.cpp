#include "Texture.h"

Texture::Texture()
{
	textureFilePath = "";
	textureID = 0;
	width = height = 0;
	bitDepth = 0;
	enableAF = true;
	AFSamplingRate = 16.0f;
}

Texture::Texture(const char* textureFile) : textureFilePath(textureFile)
{
	textureID = 0;
	width = height = 0;
	bitDepth = 0;
	enableAF = true;
	AFSamplingRate = 16.0f;
}

Texture::~Texture()
{

}

void Texture::LoadTexture()
{
	unsigned char* textureData = stbi_load(textureFilePath, &width, &height, &bitDepth, 0);

	if (!textureData)
	{
		std::cout << "Failed to load texture from " << textureFilePath << "." << std::endl;
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (enableAF)
	{
		float AFRateApplied = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &AFRateApplied);
		AFRateApplied = std::fminf(AFRateApplied, AFSamplingRate);
		std::cout << "Applied AF Sampling Rate: " << AFRateApplied << std::endl;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, AFRateApplied);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(textureData);
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = height = 0;
	bitDepth = 0;
	textureFilePath = "";
}