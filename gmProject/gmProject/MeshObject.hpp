#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#define MESH_FOLDER "Meshes/"

struct MtlContainer
{
	GLfloat Ns; // specular exponent
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	std::string filename;
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

	GLuint numberOfPoints;
	GLuint numberOfIndicies;

	MtlContainer mtl;

	GLuint offset;
	GLuint offsetInd;

	bool loadObj(std::string filename, std::string& mtlFileName, 
	std::vector<Point>& points, std::vector<GLuint>& indices);
	
	bool loadMtl(std::string filename, std::string& textureFileName, 
	MtlContainer& mtl);
	
	bool loadTexture(std::string filename);

public:

	MeshObject(std::string filename);
	MeshObject(std::vector<Point> points);
	~MeshObject(){}

	void FreeMemory();

	void SetOffset(GLuint offset);
	void SetOffsetInd(GLuint offset);

	void AddIndicies(GLuint add);

	GLuint GetNumberOfPoint() const;
	GLuint GetNumberOfIndicies() const;
	GLuint GetOffset() const;
	GLuint GetOffsetInd() const;
	std::vector<Point> GetPoints() const;
	GLuint GetFloatAmount() const;
	std::vector<GLuint> GetIndicies() const;
	GLuint GetGLuintAmount() const;
	MtlContainer GetMtl() const;
};

