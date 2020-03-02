#include "..\include\Material.h"


Material::Material(GLfloat sIntensity, GLfloat shininess) :
	shine{ shininess }, specularIntensity{ sIntensity }
{
}

void Material::applyMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shine);
}

Material::~Material()
{
}
