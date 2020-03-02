#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>


class Light
{
public:
	Light();
	Light(GLfloat red,        GLfloat green,      GLfloat blue,       GLfloat aIntensity, 
		  GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat dIntensity);

	void Illuminate(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
					GLuint lightDirectionLocation,   GLuint diffuseIntensityLocation);

	~Light();

private:
	glm::vec3 color, lightDirection;
	GLfloat ambientIntensity, diffuseIntentsity;

};