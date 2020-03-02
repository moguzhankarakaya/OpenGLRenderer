#include "..\include\Light.h"

Light::Light()
{
	color             = glm::vec3(1.0f, 1.0f, 1.0f);
	lightDirection    = glm::vec3(0.0f, -1.0f, 0.0f);
	ambientIntensity  = 1.0f;
	diffuseIntentsity = 0.0f;
}

Light::Light(GLfloat red,        GLfloat green,      GLfloat blue,       GLfloat aIntensity,
			 GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat dIntensity)
{
	color             = glm::vec3(red, green, blue);
	lightDirection    = glm::vec3(xDirection, yDirection, zDirection);
	ambientIntensity  = aIntensity;
	diffuseIntentsity = dIntensity;
}

void Light::Illuminate(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
					   GLuint lightDirectionLocation,   GLuint diffuseIntensityLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(lightDirectionLocation, lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntentsity);
}

Light::~Light()
{
}
