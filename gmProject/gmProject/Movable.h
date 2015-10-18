#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Movable
{
protected:
	glm::mat4 translationMatrix;
public:
	Movable();
	~Movable();

	void SetPosition(vec3 position);
	void UpdatePosition(vec3 change);
	vec3 GetPosition() const;
};

