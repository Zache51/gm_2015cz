#include "Camera.hpp"

glm::mat4 Camera::GetViewMatrix() const
{
	return rotationMatrix * translationMatrix;
}

Camera::Camera()
{
	rotationMatrix = glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f));
	this->SetPosition(vec3(0.0f, 0.0f, 0.0f));
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

void Camera::SetScreenSize(float height, float width)
{
	projectionMatrix = glm::perspective(3.14f * 0.45f, width / height, 0.5f, 500.0f);
}

void Camera::SetRotationMatrix(glm::mat4 rotation)
{
	rotationMatrix = rotation;
}

void Camera::SetX(float x)
{
	translationMatrix[3][0] = x;
}

void Camera::SetY(float y)
{
	translationMatrix[3][1] = y;
}

void Camera::SetZ(float z)
{
	translationMatrix[3][2] = z;
}