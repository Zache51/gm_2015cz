#include "HeightMap.h"

/******************************* Private *******************************/

GLfloat HeightMap::setVertexColor(int x, int y)
{
	GLfloat color = -0.15f + (getHeight(x, y) / 256.0f);
	return color;
}

/***********************************************************************/
/******************************* Public ********************************/

HeightMap::HeightMap(std::string filename, const Camera* cam)
{
	modelMatrix = glm::mat4(1.0);

	mapWidth = 8;
	mapDepth = 8;
	mapSize = mapWidth * mapDepth;
	g_HeightMap = new unsigned char[mapSize];

	rgbColor = 1.0f;

	qLevels = 2;

	//createViewFrustum(cam);
	
	// Load the points and save them until loaded by GC
	points = std::vector<Point_HeightMap>();
	loadRawFile(MESH_FOLDER + filename);
	numberOfPoints = points.size();

	// Create the indicies and save them until loaded by GC
	//indicies = std::vector<GLuint>();
	//quadTree = createQuadTree(qLevels, 0.0f, 0.0f, mapWidth, mapDepth);
	numberOfIndicies = indicies.size();
}

HeightMap::~HeightMap()
{
	//delete g_HeightMap;

	//releaseQuadTree(quadTree);
}

void HeightMap::FreeMemory()
{
	points.clear();
	indicies.clear();
}

/////////// --- HEIGHT MAP FUNCTIONS --- ///////////
bool HeightMap::loadRawFile(std::string filename)
{
	bool loadFromFile = false;
	FILE* file = nullptr;

	// Opens the file in Read/Binary mode.
	file = fopen(filename.data(), "rb");

	// Check if file was found and could open it
	if (file != nullptr)
	{
		// Read the .raw file into data array.
		fread(g_HeightMap, 1, mapSize, file);

		// Check for data error
		int result = ferror(file);
		//result = errno;
		if (result) loadFromFile = false;
		else loadFromFile = true;

	}
	else
	{
		// Height map not found
		loadFromFile = false;
	}
	fclose(file);
	
	if (loadFromFile)
	{
		Point_HeightMap temp;
		GLfloat loopDepth = mapDepth - 1;
		GLfloat loopWidth = mapWidth - 1;
		for (int z = 0; z < loopDepth; z++)
		{
			for (int x = 0; x < loopWidth; x++)
			{
				if (z == 0 || z == 1 || z == 2)
				{
					GLuint topleft = x + z * mapWidth;
					GLuint topright = topleft + 1;
					GLuint bottomleft = topleft + mapWidth;

					indicies.push_back(bottomleft);
					indicies.push_back(topright);
					indicies.push_back(topleft);

					indicies.push_back(bottomleft);
					indicies.push_back(bottomleft + 1); // bottomright
					indicies.push_back(topright);
				}

				// Vertices fot whole heightmap exept for right and bottom edges
				temp.ver = glm::vec3(x*4,getHeight(x, z) / 8 /*-10*/, z*4);
				temp.col = glm::vec3(1, 1, 0);

				if (x == 0 && z == 0)
				{
					temp.col = glm::vec3(0, 0, 1);
				}

				points.push_back(temp);
			}

			//indicies.push_back(loopDepth + z * mapDepth);
			//indicies.push_back(loopDepth + ((z + 1) * mapDepth));
			//indicies.push_back(loopDepth + ((z + 1) * mapDepth));

			//indicies.push_back(loopDepth + ((z + 1) * mapDepth));
			//indicies.push_back(loopDepth + ((z + 1) * mapDepth));
			//indicies.push_back(mapDepth + z * mapDepth);

			//indicies.push_back(loopDepth + ((z + 1) * mapDepth));
			//indicies.push_back(loopDepth + ((z + 1) * mapDepth));
			//indicies.push_back(mapDepth + z * mapDepth);

			if (z == 0)
			{
				indicies.push_back(7);
				indicies.push_back(15);
				indicies.push_back(15);

				indicies.push_back(15);
				indicies.push_back(15);
				indicies.push_back(8);

				indicies.push_back(15);
				indicies.push_back(8);
				indicies.push_back(8);
			}

			if (z == 1)
			{
				indicies.push_back(7 + 8);
				indicies.push_back(15 + 8);
				indicies.push_back(15 + 8);

				indicies.push_back(15 + 8);
				indicies.push_back(15 + 8);
				indicies.push_back(8 + 8);

				indicies.push_back(15 + 8);
				indicies.push_back(8 + 8);
				indicies.push_back(8 + 8);
			}

			// Vertices for right edge
			temp.ver = glm::vec3(loopWidth*4, getHeight(loopWidth, z) / 8/*-10*/, z*4);
			temp.col = glm::vec3(1, 1, 0);

			if (z == 0)
			{
				temp.col = glm::vec3(1, 0, 0);
			}

			points.push_back(temp);
		}
		for (int z = 0; z < mapDepth; z++)
		{
			// Vertices for bottom edge
			temp.ver = glm::vec3(loopWidth*4, getHeight(loopWidth, loopDepth) / 8/*-10*/, loopDepth*4);
			temp.col = glm::vec3(1, 1, 0);
			points.push_back(temp);
		}

	}

	return loadFromFile;
}
int HeightMap::getHeight(int x, int z)
{
	// Check if empty
	if (!g_HeightMap)
	{
		fprintf(stdout, "----------- Error log Height -------------\n");
		fprintf(stdout, "g_HeightMap is empty\n");
		return 0;
	}

	// Force x and y to cap at (mapSize - 1)
	int _x = x % (int)mapWidth;
	int _z = z % (int)mapDepth;

	// Treat the array like a 2D array (.raw format is a single array)
	return g_HeightMap[(int)(_x + (_z * mapDepth))] - 250;	// Index into our height array and return the height
}

//////////////////////////////////////////////////////////////

// Needs to be reworked

void* HeightMap::GetPointsData()
{
	return points.data();
}
GLuint HeightMap::GetFloatAmount() const
{
	return numberOfPoints * sizeof(Point_HeightMap);
}


///////////////////////////View frustum/////////////////////////

int HeightMap::GetRenderCount() const
{
	return renderCount;
}

void HeightMap::createViewFrustum(const Camera* cam)
{
	glm::vec2 tileScale = cam->GetScreenSize() * (1.0f / cam->GetScreenSize());
	glm::vec2 tileBias = tileScale - glm::vec2(1.0f, 1.0f);

	glm::mat4 proj = cam->GetProjectionMatrix();

	glm::vec4 c1 = glm::vec4(-proj[0][0] * tileScale.x, 0.0f, tileBias.x, 0.0f);
	glm::vec4 c2 = glm::vec4(0.0f, -proj[1][1] * tileScale.y, tileBias.y, 0.0f);
	glm::vec4 c3 = glm::vec4(0.0f, 0.0f, -proj[2][2], 0.0f);
	glm::vec4 c4 = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);


	frustumPlanes[0] = c4 - c1;								// Right
	frustumPlanes[1] = c4 + c1;								// Left

	frustumPlanes[2] = c4 - c2;								// Bottom
	frustumPlanes[3] = c4 + c2;								// Top

	frustumPlanes[4] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) - c3;// Near
	frustumPlanes[5] = glm::vec4(0.0f, 0.0f, 0.0f, 1000.0f) + c3;// Far

	for (int i = 0; i < 6; i++)
	{
		frustumPlanes[i] *= (1.0f / frustumPlanes[i].length());
	}

}
int count = 0;
QuadTree* HeightMap::createQuadTree(int levels, GLfloat startX, GLfloat startY, GLfloat endX, GLfloat endY)
{
	QuadTree* root = new QuadTree();

	GLfloat x = ((endX - startX) * 0.5f) + startX;
	GLfloat y = ((endY - startY) * 0.5f) + startY;
	GLfloat size = abs((endX - startX) * 0.5f);

	root->x = x;
	root->y = y;
	root->size = size;

	root->q_IndexBuffer = 0;
	if (levels != 0)
	{
		root->topLeft = createQuadTree(levels - 1, x - size, y, x, y - size);
		root->topRight = createQuadTree(levels - 1, x, y - size, x + size, y);
		root->botLeft = createQuadTree(levels - 1, x - size, y, x, y + size);
		root->botRight = createQuadTree(levels - 1, x, y, x + size, y + size);
	}
	else
	{
		root->topLeft = nullptr;
		root->topRight = nullptr;
		root->botLeft = nullptr;
		root->botRight = nullptr;

		root->bufferOffset = indicies.size();

		int i_size = static_cast<int>(size);
		int countSizeX = (i_size * 2);
		int countSizeY = (i_size * 2);

		int xOffset = static_cast<int>(abs(x - i_size));
		int yOffset = static_cast<int>(abs(y - i_size));


		// offset fix for last triangle row
		if (mapWidth - countSizeX == xOffset)
		{
			countSizeX--;
		}
		if (mapWidth - countSizeY == yOffset)
		{
			countSizeY--;
		}

		GLuint vertexIndex;
		GLuint max;
		int asdf = 0;
		for (int _w = 0; _w < countSizeX; _w++)
		{
			for (int _h = 0; _h < countSizeY; _h++)
			{
				vertexIndex = ((_w + xOffset) * mapWidth) + (_h + yOffset);

				indicies.push_back(vertexIndex);
				indicies.push_back(vertexIndex + mapWidth + 1);
				indicies.push_back(vertexIndex + 1);
				indicies.push_back(vertexIndex);
				indicies.push_back(vertexIndex + mapWidth);
				indicies.push_back(vertexIndex + mapWidth + 1);

				max = vertexIndex + mapWidth + 1;

				if (max >= 1023)
				{
					int asfd = 0;
				}
				if (vertexIndex >= 1023)
				{
					int asfd = 0;
				}

				root->nrIndex += 6;
			}
		}

		count++;

	}
	return root;
}

void HeightMap::checkQuadTree(QuadTree* qt, Camera* cam)
{
	float size = qt->size;
	glm::vec4 posOffset[] = {
		glm::vec4(size, size, size, 0.0f),
		glm::vec4(size, size, -size, 0.0f),
		glm::vec4(-size, size, size, 0.0f),
		glm::vec4(-size, size, -size, 0.0f),
	};


	// AABB in frustum
	bool inside = false;
	for (int o = 0; o < 4 && !inside; o++)
	{

		glm::vec4 pos = cam->GetViewMatrix() * (glm::vec4(qt->x, qt->z, qt->y, 1.0f) + posOffset[o]);
		float dist;
		float rad = 25.0f;
		bool inFrustum = true;
		for (int i = 4; i >= 0 && inFrustum; i--)
		{
			// AABB is infinite in height
			if (i == 2 || i == 3) continue;
			dist = glm::dot(frustumPlanes[i], pos);
			inFrustum = (-rad <= dist);
		}

		dist = glm::dot(frustumPlanes[5], pos);

		// far plane check
		if (dist < -rad)
		{
			inFrustum = false;
		}

		inside = inFrustum;
	}

	// frustum inside AABB
	if (!inside)
	{
		glm::vec4 point0 = (glm::vec4(qt->x, qt->z, qt->y, 1.0f) + posOffset[0]);
		glm::vec4 point3 = (glm::vec4(qt->x, qt->z, qt->y, 1.0f) + posOffset[3]);

		glm::vec4 camPos = glm::vec4(cam->GetPosition(), 1.0f);
		float camX = -camPos[0];
		float camY = -camPos[2];
		if (camX < point0.x && camX > point3.x && camY < point0.z && camY > point3.z)
		{
			inside = true;
		}
	}

	if (qt->botLeft)
	{
		checkQuadTree(qt->botLeft, cam);
		checkQuadTree(qt->botRight, cam);
		checkQuadTree(qt->topLeft, cam);
		checkQuadTree(qt->topRight, cam);
	}

	qt->visible = true;
}

void HeightMap::renderQuadTree(QuadTree* qt)
{

	if (qt->botLeft)
	{
		renderQuadTree(qt->botLeft);
		renderQuadTree(qt->botRight);
		renderQuadTree(qt->topLeft);
		renderQuadTree(qt->topRight);
	}
	else
	{
		if (qt->visible)
		{
			renderCount++;
			glDrawElements(GL_LINE_STRIP, 12 * qt->nrIndex, GL_UNSIGNED_INT, (void*)qt->bufferOffset);
			//glDrawElements(GL_TRIANGLES, qt->nrIndex, GL_UNSIGNED_INT, (void*)qt->bufferOffset);
		}
	}
}

void HeightMap::RenderHeightMap(Camera* cam)
{
	renderCount = 0;
	checkQuadTree(quadTree, cam);
	renderQuadTree(quadTree);
}

void HeightMap::releaseQuadTree(QuadTree* qt)
{
	if (qt->botLeft)
	{
		releaseQuadTree(qt->botLeft);
		releaseQuadTree(qt->botRight);
		releaseQuadTree(qt->topLeft);
		releaseQuadTree(qt->topRight);
	}
	delete qt;
}