#pragma once

#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();
	
	void CreateFromString(const char* vertexShaderCode, const char* fragmentShaderCode);
	void CreateFromFiles(const char* vertexShaderPath, const char* fragmentShaderPath);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();

	void UseShader();
	void ClearShader();
	 
	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView;

	void CompileShader(const char* vertexShaderCode, const char* fagmantShaderCode);
	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
	std::string ReadFile(const char* filePath);
};

