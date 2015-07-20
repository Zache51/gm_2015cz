#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Camera
{
	//float rx, ry, rz;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	float width, height;

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(position, 
			glm::vec3(0, 0, 0), 
			glm::vec3(0, 2, 0));
	}

	//glm::mat4 translation;
	//glm::mat4 projection;
	//glm::mat3 rot;

	//void SetRotation()
	//{
	//	glm::quat quatx = glm::angleAxis(rx, glm::vec3(1, 0, 0));
	//	glm::quat quaty = glm::angleAxis(ry, glm::vec3(0, 1, 0));

	//	rot = glm::mat3_cast(glm::cross(quatx, quaty));
	//}
};

#endif