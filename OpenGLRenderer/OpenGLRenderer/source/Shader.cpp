#include "Shader.h"



Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0; 
	uniformProjection = 0;
}


void Shader::CreateFromString(const char* vertexShaderCode, const char* fragmentShaderCode)
{
	CompileShader(vertexShaderCode, fragmentShaderCode);
}

void Shader::CreateFromFiles(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexShaderCode   = ReadFile(vertexShaderPath);
	std::string fragmentShaderCode = ReadFile(fragmentShaderPath);

	const char* vertexCodeCharArray   = vertexShaderCode.c_str();
	const char* fragmentCodeCharArray = fragmentShaderCode.c_str();

	CompileShader(vertexCodeCharArray, fragmentCodeCharArray);
}

std::string Shader::ReadFile(const char* filePath)
{
	std::string shaderCode;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "Reading shader file has failed. File " << filePath << " doesn't exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		shaderCode.append(line + "\n");
	}

	fileStream.close();
	return shaderCode;
}

void Shader::CompileShader(const char* vertexShaderCode, const char* fagmantShaderCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		std::cout << "Shader program compilation has failed!" << std::endl;
		return;
	}

	AddShader(shaderID, vertexShaderCode,  GL_VERTEX_SHADER  );
	AddShader(shaderID, fagmantShaderCode, GL_FRAGMENT_SHADER);

	GLint  _result = 0;
	GLchar _eCode[1024] = { 0 };

	glLinkProgram(shaderID);

	glGetProgramiv(shaderID, GL_LINK_STATUS, &_result);
	if (!_result)
	{
		glGetProgramInfoLog(shaderID, sizeof(_eCode), NULL, _eCode);
		std::cout << "Error happened while linking program " << _eCode << std::endl;
		return;
	}

	glValidateProgram(shaderID);

	glGetProgramiv(shaderID, GL_LINK_STATUS, &_result);
	if (!_result)
	{
		glGetProgramInfoLog(shaderID, sizeof(_eCode), NULL, _eCode);
		std::cout << "Error happened while validating program " << _eCode << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
	GLuint  s = glCreateShader(shaderType);
	const GLchar* c[1];
	c[0] = shaderCode;

	GLint l[1];
	l[0] = strlen(shaderCode);
	glShaderSource(s, 1, c, l);
	glCompileShader(s);

	GLint  _result = 0;
	GLchar _eCode[1024] = { 0 };

	glGetShaderiv(s, GL_COMPILE_STATUS, &_result);
	if (!_result)
	{
		glGetShaderInfoLog(s, sizeof(_eCode), NULL, _eCode);
		std::cout << "Error happened while compuling " << shaderType << " shader" << _eCode << std::endl;
		return;
	}

	glAttachShader(program, s);
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);

}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteShader(shaderID); 
		shaderID = 0;
	}

	uniformModel      = 0;
	uniformProjection = 0;
}


Shader::~Shader()
{
	ClearShader();
}
