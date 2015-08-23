#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include <errno.h>
#include <stdio.h>

#define MESH_FOLDER "Meshes/"

struct QuadTree
{
	GLfloat x, y, z = 100;	//Center
	GLfloat size;			//offset
	GLuint q_IndexBuffer;
	GLuint nrIndex;

	bool visible = false;

	QuadTree* topLeft;
	QuadTree* topRight;
	QuadTree* botLeft;
	QuadTree* botRight;

};

class HeightMap
{
private:


	int mapWidth;				// Width of .raw height map
	int mapHeight;				// Height of .raw height map
	int mapSize;				// Total size of height map
	
	unsigned char* g_HeightMap;	// Holds the height maps raw data

	int gridWidth;				// Defined width for actual map
	int gridHeight;				// Defined height for actual map

	struct VertexPosition
	{
		GLfloat x, y, z;
		GLfloat r, g, b;
	};
	VertexPosition* vertices;
	GLfloat rgbColor;
	GLfloat setVertexColor(int, int);	// Set color of the map

	float ry = 0;
	glm::mat4 modelMatrix;

	
	// Quad tree and frustum stuff

	//glm::vec4 frustumPlanes[6];

	//int qLevels;
	int quadSize;				// Width and height of each QUAD

	//QuadTree* quadTree;
	//void renderQuadTree(QuadTree* qt);
	//void releaseQuadTree(QuadTree* qt);
	//QuadTree* createQuadTree(int levels, GLfloat startX, GLfloat startY, GLfloat endX, GLfloat endY);
	//void checkQuadTree(QuadTree* qt, glm::mat4);
	

public:
	HeightMap();
	~HeightMap();

	bool loadRawFile(const char* filename);
	int getHeight(int x, int y);
	//void CreateViewFrustum(glm::mat4 proj, glm::mat4 view, glm::vec2 screenSize);
	int renderCount;
};

