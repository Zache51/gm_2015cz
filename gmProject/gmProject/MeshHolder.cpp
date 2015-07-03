#include "MeshHolder.hpp"


MeshHolder::MeshHolder()
{
}


MeshHolder::~MeshHolder()
{
}

void MeshHolder::SetRotation(mat4 rotation)
{
	rotationMatrix = rotation;
}

void MeshHolder::SetTranslation(mat4 translation)
{
	translationMatrix = translation;
}

mat4 MeshHolder::GetWorld() const
{
	return translationMatrix * rotationMatrix;
}