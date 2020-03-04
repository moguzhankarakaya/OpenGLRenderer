#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <string.h>
#include <iostream>
#include <vector> 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Light.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"

const GLint  WIDTH = 800, HEIGHT = 600;
const float toRadians = glm::pi<float>() / 180.0f;
 
Window*   mainWindow;
Camera*   fpsCamera;
Light*    sunLight;
Material* metalicMaterial;
Material* dullMaterial;
std::vector<Mesh*> meshes;
std::vector<Shader*> shaders;
std::vector<Texture*> textures;

GLfloat deltaTime = 0.0f;
GLfloat lastTime  = 0.0f;
GLfloat currTime  = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/VertexShader.vs";

// Pixel Shader
static const char* fShader = "Shaders/FragmentShader.ps";

void calcAverageNormals(unsigned int* indices,  unsigned int indecisCount,
						GLfloat*      vertices, unsigned int verticesCount, 
					    unsigned int vLength,   unsigned int normalOffset)
{
	for (size_t i = 0; i < indecisCount; i += 3)
	{
		unsigned int v0idx = indices[i]   * vLength;
		unsigned int v1idx = indices[i+1] * vLength;
		unsigned int v2idx = indices[i+2] * vLength;

		glm::vec3 edge1(vertices[v1idx] - vertices[v0idx], vertices[v1idx + 1] - vertices[v0idx + 1], vertices[v1idx + 2] - vertices[v0idx + 2]);
		glm::vec3 edge2(vertices[v2idx] - vertices[v0idx], vertices[v2idx + 1] - vertices[v0idx + 1], vertices[v2idx + 2] - vertices[v0idx + 2]);
		glm::vec3 normal = glm::cross(edge1, edge2);
		normal = glm::normalize(normal);

		v0idx += normalOffset; v1idx += normalOffset; v2idx += normalOffset;
		vertices[v0idx] += normal.x; vertices[v0idx + 1] += normal.y; vertices[v0idx + 2] += normal.z;
		vertices[v1idx] += normal.x; vertices[v1idx + 1] += normal.y; vertices[v1idx + 2] += normal.z;
		vertices[v2idx] += normal.x; vertices[v2idx + 1] += normal.y; vertices[v2idx + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		unsigned int normalIDX = i * vLength + normalOffset;
		glm::vec3 vec(vertices[normalIDX], vertices[normalIDX + 1], vertices[normalIDX + 2]);
		vec = glm::normalize(vec);
		vertices[normalIDX] = vec.x; vertices[normalIDX + 1] = vec.y; vertices[normalIDX + 2] = vec.z;
	}
}

void CreateTriangle()
{
	unsigned int indices[] = { 0, 1, 2 };

	GLfloat vertices[] = { -1.0f, -1.0f, 0.0f,
							1.0f, -1.0f, 0.0f,
							0.0f,  1.0f, 0.0f };

	Mesh* triangle = new Mesh();
	triangle->CreateMesh(vertices, indices, 9, 3);

	meshes.push_back(triangle);
}

void CreatePyramid()
{
	unsigned int indices[] = { 0, 2, 1,
							   1, 2, 3,
							   0, 1, 4,
							   1, 3, 4,
							   3, 2, 4,
							   2, 0, 4, };
						 //  x      y     z      u      v      nx     ny     nz
	GLfloat vertices[] = { -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
						    1.0f, -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
						   -1.0f, -1.0f, 2.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
							1.0f, -1.0f, 2.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
							0.0f,  0.5f, 1.0f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f, };

	calcAverageNormals(indices, 18, vertices, 40, 8, 5);

	Mesh* pyramid1 = new Mesh();
	pyramid1->CreateMesh(vertices, indices, 40, 18);
	Mesh* pyramid2 = new Mesh();
	pyramid2->CreateMesh(vertices, indices, 40, 18);

	meshes.push_back(pyramid1);
	meshes.push_back(pyramid2);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaders.push_back(shader1);
}

int main() 
{
	mainWindow = new Window(WIDTH, HEIGHT, "TESTING");
	mainWindow->initialise();

	fpsCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 20.0f); 

	sunLight = new Light(1.0f,  1.0f,  1.0f, 0.5f, 
						 2.0f, -1.0f, -2.0f, 1.0f);

	metalicMaterial = new Material(1.0f, 32.0f);
	dullMaterial    = new Material(1.0f, 4.0f);

	textures.push_back(new Texture("assets/brick.png"));
	textures.back()->LoadTexture();
	textures.push_back(new Texture("assets/dirt.png"));
	textures.back()->LoadTexture();

	// Set GL Options
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// Create Demo Object
	CreatePyramid();
	
	// Compile Shaders
	CreateShaders();

	// Create projection matrix and model transformation matrix
	glm::vec3 eyePosition;
	glm::mat4 model;
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformAmbientIntensity = 0, uniformAmbientColor = 0, 
		uniformLightDirection = 0, uniformDiffuseIntensity = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow->getBufferWidth() / (GLfloat)mainWindow->getBufferHeight(), 0.1f, 100.0f);

	// Loop until it is closed
	while (!mainWindow->getShoudlClose())
	{
		currTime = (GLfloat)glfwGetTime();
		deltaTime = currTime - lastTime;
		lastTime = currTime;
		
		// Get and handle user inputs
		glfwPollEvents();

		fpsCamera->keyControl(mainWindow->getKeys(), deltaTime);
		fpsCamera->mouseControl(mainWindow->getMouseDeltaX(), mainWindow->getMouseDeltaY(), deltaTime);
		
		eyePosition = fpsCamera->getCameraPosition();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[0]->UseShader();
		uniformModel             = shaders[0]->GetModelLocation();
		uniformProjection        = shaders[0]->GetProjectionLocation();
		uniformView              = shaders[0]->GetViewLocation();
		uniformEyePosition       = shaders[0]->GetEyePositionLocation();
		uniformAmbientColor      = shaders[0]->GetAmbientColorLocation();
		uniformAmbientIntensity  = shaders[0]->GetAmbientIntensityLocation(); 
		uniformDiffuseIntensity  = shaders[0]->GetDiffuseIntensityLocation();
		uniformLightDirection    = shaders[0]->GetLightDirectionLocation();
		uniformSpecularIntensity = shaders[0]->GetSpecularIntensityLocation();
		uniformShininess         = shaders[0]->GetShininessLocation();

		sunLight->Illuminate(uniformAmbientIntensity, uniformAmbientColor, uniformLightDirection, uniformDiffuseIntensity);
		dullMaterial->applyMaterial(uniformSpecularIntensity, uniformShininess);

			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(fpsCamera->getViewMatrix()));
			glUniform3f(uniformEyePosition, eyePosition.x, eyePosition.y, eyePosition.z);

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-0.5f, 0.5f, -2.0f));
			//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			//model = glm::rotate(model, 15.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			textures[0]->UseTexture();
			meshes[0]->RenderMesh();

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(1.5f, -1.5f, -2.0f));
			//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			//model = glm::rotate(model, 15.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			textures[1]->UseTexture(); 
			meshes[1]->RenderMesh();


		glUseProgram(0);

		mainWindow->swapBuffers();
	}

	return 0;
}