#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
private:
	glm::vec3 position;
	
	float width, height;

	glm::mat4 projectionMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 translationMatrix;
	
	glm::mat4 getViewMatrix() const;

public:
	// temporary public
	float rx, ry, rz;

	Camera();

	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetPVMatrix() const;
	
	// One of these is probably redundant
	void UpdatePosition(glm::vec3 change);
	void UpdateTranslation(glm::vec3 change);

	void SetPosition(glm::vec3 position);
	void SetScreenSize(float height, float width);
	void SetRotationMatrix(glm::mat4 rotation);
	void SetTranslationMatrix(glm::mat4 translation);
};

#endif