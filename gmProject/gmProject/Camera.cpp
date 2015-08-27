#include "Camera.hpp"

glm::mat4 Camera::GetViewMatrix() const
{
	return rotationMatrix * translationMatrix;
}

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);	
	rotationMatrix = glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f));
	translationMatrix = glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetRotationMatrix() const
{
	return rotationMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

glm::mat4 Camera::GetPVMatrix() const
{
	return projectionMatrix * GetViewMatrix();
}

glm::vec2 Camera::GetScreenSize() const
{
	return glm::vec2(width, height);
}

void Camera::UpdatePosition(glm::vec3 change)
{
	position += change;
	translationMatrix = glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Camera::UpdateTranslation(glm::vec3 change)
{
	translationMatrix = glm::translate(translationMatrix, change);
}

void Camera::SetPosition(glm::vec3 position)
{
	this->position = position;
	translationMatrix = glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Camera::SetScreenSize(float height, float width)
{
	this->height = height;
	this->width = width;
	projectionMatrix = glm::perspective(3.14f * 0.45f, width / height, 0.5f, 500.0f);
}

void Camera::SetRotationMatrix(glm::mat4 rotation)
{
	rotationMatrix = rotation;
}

void Camera::SetTranslationMatrix(glm::mat4 translation)
{
	translationMatrix = translation;
}