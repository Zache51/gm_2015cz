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
	
	std::vector<std::string> filenames;
	std::vector<GLuint> textureIDs;
};

class MeshObject : public MeshBase
{
private:

	float scale;
	std::vector<Point_Obj> points;

	GLuint offset;
	GLuint offsetInd;

	bool loadObj(std::string filename, std::string& mtlFileName);	
	bool loadMtl(std::string filename, MtlContainer& mtl);
	bool loadTexture(MtlContainer& mtl);

public:
	MtlContainer mtl;

	MeshObject();
	MeshObject(std::string filename, float scale);
	~MeshObject(){}

	void FreeMemory();

	void SetOffset(GLuint offset);
	void SetOffsetInd(GLuint offset);

	void AddIndicies(GLuint add);

	int GetScale() const;

	GLuint GetNumberOfPoint() const;
	GLuint GetOffset() const;
	GLuint GetOffsetInd() const;
	
	void* GetPointsData();
	GLuint GetFloatAmount() const;
};

#endif