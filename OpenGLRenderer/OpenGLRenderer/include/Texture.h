#pragma once
#include <iostream>

#include <GL\glew.h>

#include <stb_image.h>

class Texture
{
public:
	Texture();
	Texture(const char* textureFile);

	void LoadTexture();
	void ClearTexture();
	void UseTexture();

	~Texture();

private:
	bool enableAF;
	GLuint textureID;
	float AFSamplingRate;
	int width, height, bitDepth;

	const char* textureFilePath;
};