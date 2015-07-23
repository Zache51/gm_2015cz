#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
private:
	//float rx, ry, rz;
	glm::vec3 position;
	glm::mat4 projectionMatrix;
	//glm::mat3 rot;
	float width, height;

public:
	Camera();

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetPVMatrix() const;

	void SetPosition(glm::vec3 position);
	void SetScreenSize(float height, float width);

	//void SetRotation()
	//{
	//	glm::quat quatx = glm::angleAxis(rx, glm::vec3(1, 0, 0));
	//	glm::quat quaty = glm::angleAxis(ry, glm::vec3(0, 1, 0));

	//	rot = glm::mat3_cast(glm::cross(quatx, quaty));
	//}
};

#endif