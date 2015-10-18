#include "MeshHolder.hpp"


MeshHolder::MeshHolder(MeshObject* mesh)
{
	this->mesh = mesh;
}

void MeshHolder::SetRotation(mat4 rotation)
{
	rotationMatrix = rotation;
}

mat4 MeshHolder::GetWorld() const
{
	return translationMatrix * rotationMatrix;
}

MeshObject* MeshHolder::GetMesh() const
{
	return mesh;
}