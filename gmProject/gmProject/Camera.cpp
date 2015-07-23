#include "Camera.hpp"


Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);;
}


glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 2, 0));
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

glm::mat4 Camera::GetPVMatrix() const
{
	return projectionMatrix * GetViewMatrix();
}

void Camera::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Camera::SetScreenSize(float height, float width)
{
	this->height = height;
	this->width = width;
	projectionMatrix = glm::perspective(3.14f * 0.45f, width / height, 0.5f, 500.0f);
}