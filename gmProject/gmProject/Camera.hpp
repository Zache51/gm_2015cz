#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_FORCE_RADIANS
#include "Movable.h"

using namespace glm;

class Camera : public Movable
{
private:
	float width, height;

	glm::mat4 projectionMatrix;
	glm::mat4 rotationMatrix;
	//glm::mat4 translationMatrix;

public:
	Camera();

	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetPVMatrix() const;

	glm::vec2 GetScreenSize() const;
	
	//void SetPosition(vec3 position);
	//void UpdatePosition(vec3 change);
	//vec3 GetPosition() const;

	void SetScreenSize(float height, float width);
	void SetRotationMatrix(glm::mat4 rotation);
	void SetTranslationMatrix(glm::mat4 translation);
};

#endif