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
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"

const GLint  WIDTH = 800, HEIGHT = 600;
const float toRadians = glm::pi<float>() / 180.0f;
 
Window* mainWindow;
Camera* fpsCamera;
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
	unsigned int indices[] = { 0, 3, 1,
							   1, 3, 2,
							   2, 3, 0,
							   0, 1, 2 };
						 //  x      y     z      u      v
	GLfloat vertices[] = { -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
						    0.0f, -1.0f, 1.0f,  0.5f,  0.0f,  
							1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
							0.0f,  1.0f, 0.0f,  0.5f,  1.0f,};

	Mesh* pyramid1 = new Mesh();
	pyramid1->CreateMesh(vertices, indices, 20, 12);
	Mesh* pyramid2 = new Mesh();
	pyramid2->CreateMesh(vertices, indices, 20, 12);

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
	glm::mat4 model;
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow->getBufferWidth() / (GLfloat)mainWindow->getBufferHeight(), 0.1f, 100.0f);


	// Loop until it is closed
	while (!mainWindow->getShoudlClose())
	{
		currTime = glfwGetTime();
		deltaTime = currTime - lastTime;
		lastTime = currTime;
		
		// Get and handle user inputs
		glfwPollEvents();

		fpsCamera->keyControl(mainWindow->getKeys(), deltaTime);
		fpsCamera->mouseControl(mainWindow->getMouseDeltaX(), mainWindow->getMouseDeltaY(), deltaTime);

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[0]->UseShader();
		uniformModel      = shaders[0]->GetModelLocation();
		uniformProjection = shaders[0]->GetProjectionLocation();
		uniformView       = shaders[0]->GetViewLocation();
			
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(fpsCamera->getViewMatrix()));

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-0.5f, 0.5f, -2.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			model = glm::rotate(model, 15.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			textures[0]->UseTexture();
			meshes[0]->RenderMesh();

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.5f, -0.5f, -2.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			model = glm::rotate(model, 15.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			textures[1]->UseTexture(); 
			meshes[1]->RenderMesh();


		glUseProgram(0);

		mainWindow->swapBuffers();
	}

	return 0;
}