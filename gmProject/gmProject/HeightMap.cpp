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

	mapWidth = 1024;
	mapHeight = 1024;
	mapSize = mapWidth * mapHeight;
	quadSize = 2;
	g_HeightMap = new unsigned char[mapSize];

	gridWidth = mapWidth / quadSize;
	gridHeight = mapHeight / quadSize;

	rgbColor = 1.0f;

	qLevels = 4;

	createViewFrustum(cam);
	
	// Load the points and save them until loaded by GC
	points = std::vector<Point2>();
	loadRawFile(MESH_FOLDER + filename);
	numberOfPoints = points.size();

	// Create the indicies and save them until loaded by GC
	indicies = std::vector<GLuint>();
	quadTree = createQuadTree(qLevels, 0.0f, 0.0f, mapWidth, mapHeight);
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

// line of doom

// line of doom ends

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

	bool started = false;
	int minHeight = 0;
	int maxHeight = 0;

	if (loadFromFile)
	{
		for (int _w = 0; _w < mapWidth; _w += quadSize)
		{
			for (int _h = 0; _h < mapHeight; _h += quadSize)
			{
				rgbColor = setVertexColor(_w, _h);

				Point2 temp;

				if (!started)
				{
					minHeight = getHeight(_w, _h);
					maxHeight = getHeight(_w, _h);
					started = true;
				}
				else
				{
					int asdf = getHeight(_w, _h);

					if (asdf < minHeight)
					{
						minHeight = asdf;
					}
					if (asdf > maxHeight)
					{
						maxHeight = asdf;
					}
				}

				


				temp.ver = glm::vec3(_w, getHeight(_w, _h), _h);
				temp.col = glm::vec3(rgbColor, rgbColor, rgbColor);

				points.push_back(temp);
			}
		}
	}

	return loadFromFile;
}
int HeightMap::getHeight(int _x, int _y)
{
	// Force x and y to cap at (mapSize - 1)
	int x = _x % (int)mapWidth;
	int y = _y % (int)mapHeight;

	// Check if empty
	if (!g_HeightMap)
	{
		// TODO: printf to console
		return 0;
	}

	// Treat the array like a 2D array (.raw format is a single array)
	return g_HeightMap[(int)(x + (y * mapHeight))];	// Index into our height array and return the height
}

//////////////////////////////////////////////////////////////

void* HeightMap::GetPointsData()
{
	return points.data();
}
GLuint HeightMap::GetFloatAmount() const
{
	return numberOfPoints;
}

char32_t* HeightMap::GetIndiciesData()
{
	return indicies.data();
}
GLuint HeightMap::GetGLuintAmount() const
{
	return numberOfIndicies;
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
		int countSizeX = (i_size * 2) / quadSize;
		int countSizeY = (i_size * 2) / quadSize;

		int xOffset = static_cast<int>(abs(x - i_size)) / quadSize;
		int yOffset = static_cast<int>(abs(y - i_size)) / quadSize;


		// offset fix for last triangle row
		if (gridWidth - countSizeX == xOffset)
		{
			countSizeX--;
		}
		if (gridWidth - countSizeY == yOffset)
		{
			countSizeY--;
		}

		for (int _w = 0; _w < countSizeX; _w++)
		{
			for (int _h = 0; _h < countSizeY; _h++)
			{
				GLuint vertexIndex = ((_w + xOffset) * gridWidth) + (_h + yOffset);

				indicies.push_back(vertexIndex);
				indicies.push_back(vertexIndex + gridWidth + 1);
				indicies.push_back(vertexIndex + 1);
				indicies.push_back(vertexIndex);
				indicies.push_back(vertexIndex + gridWidth);
				indicies.push_back(vertexIndex + gridWidth + 1);

				root->nrIndex += 6;
			}
		}
	}
	return root;
}

void HeightMap::checkQuadTree(QuadTree* qt, glm::mat4 viewmatrix)
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

		glm::vec4 pos = viewmatrix * (glm::vec4(qt->x, qt->z, qt->y, 1.0f) + posOffset[o]);
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

		glm::vec4 camPos = viewmatrix[3];
		float camX = -camPos[0];
		float camY = -camPos[2];
		if (camX < point0.x && camX > point3.x && camY < point0.z && camY > point3.z)
		{
			inside = true;
		}
	}

	if (qt->botLeft)
	{
		checkQuadTree(qt->botLeft, viewmatrix);
		checkQuadTree(qt->botRight, viewmatrix);
		checkQuadTree(qt->topLeft, viewmatrix);
		checkQuadTree(qt->topRight, viewmatrix);
	}
	qt->visible = inside;
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
			glDrawElements(GL_TRIANGLES, 12 * qt->nrIndex, GL_UNSIGNED_INT, (void*)qt->bufferOffset);
			//glDrawElements(GL_TRIANGLES, qt->nrIndex, GL_UNSIGNED_INT, (void*)qt->bufferOffset);
		}
	}
}

void HeightMap::RenderHeightMap(Camera* cam)
{
	renderCount = 0;
	checkQuadTree(quadTree, cam->GetViewMatrix());
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