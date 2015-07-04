#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
//#include <stb_image.h>

#define MESH_FOLDER "Meshes/"

struct MtlContainer
{
	GLfloat Ns; // specular exponent
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	char* filename;
};

class MeshObject
{
private:

	struct Point
	{
		glm::vec3 ver;
		glm::vec3 vns;
	};

	MtlContainer mtl;

	bool loadObj(const char* filename, char* mtlFileName, 
	std::vector<Point>& points, std::vector<GLuint>& indices);
	
	
	bool loadMtl(const char* filename, MtlContainer& mtl);
	bool loadTexture(const char* filename);

public:

	MeshObject(const char* filename);
	~MeshObject(){}

	MtlContainer GetMtl() const;
};

