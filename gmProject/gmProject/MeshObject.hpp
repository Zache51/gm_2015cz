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

struct Point
{
	glm::vec3 ver;
	glm::vec2 uvs;
	glm::vec3 vns;
};

class MeshObject
{
private:

	std::vector<Point> points;
	std::vector<GLuint> indicies;

	MtlContainer mtl;

	bool loadObj(std::string filename, std::string mtlFileName, 
	std::vector<Point>& points, std::vector<GLuint>& indices);
	
	
	bool loadMtl(std::string filename, MtlContainer& mtl);
	bool loadTexture(std::string filename);

public:

	MeshObject(std::string filename);
	MeshObject(std::vector<Point> points);
	~MeshObject(){}

	std::vector<Point> GetPoints() const;
	GLuint GetFloatAmount() const;
	std::vector<GLuint> GetIndicies() const;
	MtlContainer GetMtl() const;
};

