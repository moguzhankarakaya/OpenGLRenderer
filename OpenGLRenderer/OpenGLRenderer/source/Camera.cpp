#include "Camera.h"

Camera::Camera(glm::vec3 initPosition, glm::vec3 initUp, GLfloat initYaw, GLfloat initPich, GLfloat initSpeedMove, GLfloat initSpeedTurn):
	position(initPosition), up(initUp), yaw(initYaw), pitch(initPich), speedMove(initSpeedMove), speedTurn(initSpeedTurn)
{
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = up;
	
	update();
}

Camera::~Camera()
{

}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up    = glm::normalize(glm::cross(right, front));
}

void Camera::keyControl(bool* keys, GLfloat elapsedTime)
 {
	if (keys[GLFW_KEY_W])
	{
		position += front * speedMove * elapsedTime;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * speedMove * elapsedTime;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * speedMove * elapsedTime;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * speedMove * elapsedTime;
	}
}

void Camera::mouseControl(GLfloat mouseDeltaX, GLfloat mouseDeltaY, GLfloat elapsedTime)
{
	yaw   += mouseDeltaX * speedTurn * elapsedTime;
	pitch += mouseDeltaY * speedTurn * elapsedTime;

	if (pitch >  89.0f) pitch =  89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
	
	update();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}
