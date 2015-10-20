#include "MeshHolder.hpp"


MeshHolder::MeshHolder(MeshObject* mesh)
{
	this->mesh = mesh;
	currentTextureID = 0;
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

GLuint MeshHolder::GetCurrentTexture() const
{
	return mesh->mtl.textureIDs[currentTextureID];
};
void MeshHolder::ChangeTexture(GLint textureIndex)
{
	int size = mesh->mtl.textureIDs.size();
	if (textureIndex >= size)
	{
		textureIndex = size - 1;
	}
	currentTextureID = textureIndex;
}