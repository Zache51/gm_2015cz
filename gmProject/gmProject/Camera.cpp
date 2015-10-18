#include "Camera.hpp"

glm::mat4 Camera::GetViewMatrix() const
{
	return rotationMatrix * translationMatrix;
}

Camera::Camera()
{
	rotationMatrix = glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f));
	translationMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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

//void Camera::SetPosition(vec3 position)
//{
//	translationMatrix = glm::translate(mat4(1.0f), position);
//}
//
//void Camera::UpdatePosition(vec3 change)
//{
//	vec3 newPos = vec3(translationMatrix[3]) + change;
//	translationMatrix[3] = vec4(newPos, 1.0f);
//}
//
//vec3 Camera::GetPosition() const
//{
//	return vec3(translationMatrix[3]);
//}

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