#ifndef MESHOBJECT_HPP
#define MESHOBJECT_HPP

#include "MeshBase.h"

#define MESH_FOLDER "Meshes/"

struct MtlContainer
{
	GLfloat Ns; // specular exponent
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	std::string filename;
	GLuint TextureID;
};

class MeshObject : public MeshBase
{
private:

	GLuint sizeOfPoint;

	
	
	// rework
	std::vector<Point_Obj> points;



	MtlContainer mtl;

	GLuint offset;
	GLuint offsetInd;

	bool loadObj(std::string filename, std::string& mtlFileName, 
		std::vector<Point_Obj>& points, std::vector<GLuint>& indices);
	
	bool loadMtl(std::string filename, MtlContainer& mtl);
	
	bool loadTexture(std::string filename);

public:

	MeshObject(std::string filename);
	MeshObject(std::vector<Point_Obj> points);
	~MeshObject(){}

	void FreeMemory();

	void SetOffset(GLuint offset);
	void SetOffsetInd(GLuint offset);

	void AddIndicies(GLuint add);

	GLuint GetNumberOfPoint() const;
	//GLuint GetNumberOfIndicies() const;
	GLuint GetOffset() const;
	GLuint GetOffsetInd() const;
	
	// Needs to be reworked
	void* GetPointsData();
	GLuint GetFloatAmount() const;
	GLuint GetGLuintAmount() const;



	MtlContainer GetMtl() const;

	GLuint GetSizeOfPoint() const;
};

#endif