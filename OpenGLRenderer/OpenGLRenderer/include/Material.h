#pragma once

#include <GL\glew.h>

class Material
{
public:
	Material(GLfloat sIntensity = 0.0f, GLfloat shininess = 0.0f);

	void applyMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat shine;
};
