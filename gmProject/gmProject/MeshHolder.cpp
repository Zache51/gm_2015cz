#include "MeshHolder.hpp"


MeshHolder::MeshHolder(MeshObject* mesh)
{
	this->mesh = mesh;
}

void MeshHolder::SetRotation(mat4 rotation)
{
	rotationMatrix = rotation;
}

//void MeshHolder::SetPosition(vec3 position)
//{
//	translationMatrix = glm::translate(mat4(1.0f), position);
//}
//
//void MeshHolder::UpdatePosition(vec3 change)
//{
//	vec3 newPos = vec3(translationMatrix[3]) + change;
//	translationMatrix[3] = vec4(newPos, 1.0f);
//}
//
//vec3 MeshHolder::GetPosition() const
//{
//	return vec3(translationMatrix[3]);
//}

mat4 MeshHolder::GetWorld() const
{
	return translationMatrix * rotationMatrix;
}

MeshObject* MeshHolder::GetMesh() const
{
	return mesh;
}