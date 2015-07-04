#include "MeshObject.hpp"



/******************************* Private *******************************/



bool MeshObject::loadObj(const char* filename, char* mtlFileName,
	std::vector<Point>& points, std::vector<GLuint>& indices)
{

}



/***********************************************************************/
/******************************* Public ********************************/



MeshObject::MeshObject(const char* filename)
{
	char mtlFilename[] = "";
	std::vector<Point> points;
	std::vector<GLuint> indicies;

	loadObj(filename, mtlFilename, points, indicies);
}

MtlContainer MeshObject::GetMtl() const
{
	return mtl;
}
