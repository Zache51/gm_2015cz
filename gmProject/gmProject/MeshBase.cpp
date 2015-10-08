#include "MeshBase.h"


MeshBase::MeshBase()
{
}


MeshBase::~MeshBase()
{
}

char32_t* MeshBase::GetIndiciesData()
{
	return indicies.data();
}
GLuint MeshBase::GetGLuintAmount() const
{
	return numberOfIndicies * sizeof(GLuint);
}

GLuint MeshBase::GetNumberOfIndicies() const
{
	return numberOfIndicies;
}
