#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Point
{
	glm::vec3 ver;
};

struct Point_Obj : public Point
{
	glm::vec2 uvs;
	glm::vec3 vns;
};

struct Point_HeightMap : public Point
{
	glm::vec3 col;
};

class MeshBase
{
protected:
	//std::vector<Point> points;
	std::vector<GLuint> indicies;

	GLuint numberOfPoints;
	GLuint numberOfIndicies;

public:
	MeshBase();
	~MeshBase();

	//void* GetPointsData();
	//GLuint GetFloatAmount() const;
	char32_t* GetIndiciesData();
	//GLuint GetGLuintAmount() const;
};

