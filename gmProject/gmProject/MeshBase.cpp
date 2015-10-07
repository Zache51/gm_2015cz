#include "MeshBase.h"


MeshBase::MeshBase()
{
}


MeshBase::~MeshBase()
{
}

//void* MeshBase::GetPointsData()
//{
//	return points.data();
//}
//GLuint MeshBase::GetFloatAmount() const
//{
//	return numberOfPoints;
//}
//
char32_t* MeshBase::GetIndiciesData()
{
	return indicies.data();
}
//GLuint MeshBase::GetGLuintAmount() const
//{
//	return numberOfIndicies;
//}

GLuint MeshBase::GetNumberOfIndicies() const
{
	return numberOfIndicies;
}
