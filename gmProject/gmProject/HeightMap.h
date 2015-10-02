#pragma once

#include "MeshBase.h"

#include <errno.h>
#include <stdio.h>

#include "Camera.hpp"

#define MESH_FOLDER "Meshes/"

struct QuadTree
{
	GLfloat x, y, z = 100;	//Center
	GLfloat size;			//offset
	GLuint q_IndexBuffer;
	GLuint bufferOffset = 0;
	GLuint nrIndex = 0;

	bool visible = false;

	QuadTree* topLeft;
	QuadTree* topRight;
	QuadTree* botLeft;
	QuadTree* botRight;

};

class HeightMap : public MeshBase
{
private:
	//rework
	std::vector<Point_HeightMap> points;



	GLfloat mapWidth;				// Width of .raw height map
	GLfloat mapHeight;				// Height of .raw height map
	int mapSize;				// Total size of height map

	unsigned char* g_HeightMap;	// Holds the height maps raw data

	int gridWidth;				// Defined width for actual map
	int gridHeight;				// Defined height for actual map

	GLfloat rgbColor;
	GLfloat setVertexColor(int, int);	// Set color of the map

	float ry = 0;
	glm::mat4 modelMatrix;


	// Quad tree and frustum stuff
	int renderCount;
	glm::vec4 frustumPlanes[6];

	int qLevels;
	int quadSize;				// Width and height of each QUAD

	QuadTree* quadTree;
	void renderQuadTree(QuadTree* qt);
	void releaseQuadTree(QuadTree* qt);
	QuadTree* createQuadTree(int levels, GLfloat startX, GLfloat startY, GLfloat endX, GLfloat endY);
	void checkQuadTree(QuadTree* qt, glm::mat4 viewmatrix);
	void createViewFrustum(const Camera* cam);


	bool loadRawFile(std::string filename);

public:
	HeightMap(std::string filename, const Camera* cam);
	~HeightMap();

	void FreeMemory();

	int getHeight(int x, int y);


	// Needs to be reworked
	void* GetPointsData();
	GLuint GetFloatAmount() const;
	GLuint GetGLuintAmount() const;



	void RenderHeightMap(Camera* cam);
	int GetRenderCount() const;
};

