#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glfw\glfw3.h>


class Camera
{
public:
	Camera(glm::vec3 initPosition, 
		   glm::vec3 initUp, 
		   GLfloat initYaw, 
		   GLfloat initPich, 
		   GLfloat initSpeedMove, 
		   GLfloat initSpeedTurn);
	~Camera();

	void keyControl(bool* keys, GLfloat elapsedTime);
	void mouseControl(GLfloat mouseDeltaX, GLfloat mouseDeltaY, GLfloat elapsedTime);

	glm::mat4 getViewMatrix();

private:
	GLfloat speedMove, speedTurn;
	glm::vec3 front, up, right;
	GLfloat yaw, pitch, roll;
	glm::vec3 position;
	glm::vec3 worldUp;

	void update();

};