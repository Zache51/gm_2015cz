#include "Movable.h"


Movable::Movable(){}

Movable::~Movable(){}

void Movable::SetPosition(vec3 position)
{
	translationMatrix = glm::translate(mat4(1.0f), position);
}

void Movable::UpdatePosition(vec3 change)
{
	vec3 newPos = vec3(translationMatrix[3]) + change;
	translationMatrix[3] = vec4(newPos, 1.0f);
}

vec3 Movable::GetPosition() const
{
	return vec3(translationMatrix[3]);
}
