#include "HeightMap.h"

/******************************* Private *******************************/

GLfloat HeightMap::setVertexColor(int x, int y)
{
	GLfloat color = -0.15f + (getHeight(x, y) / 256.0f);
	return color;
}

/***********************************************************************/
/******************************* Public ********************************/

HeightMap::HeightMap(std::string filename)
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

	//qLevels = 4;

	points = std::vector<Point2>();
	//indicies = std::vector<GLuint>();

	loadRawFile(MESH_FOLDER + filename);

	numberOfPoints = points.size();
	//numberOfIndicies = indicies.size();
}


HeightMap::~HeightMap()
{
	//delete g_HeightMap;

	//releaseQuadTree(quadTree);
}

/////////// --- HEIGHT MAP FUNCTIONS --- ///////////
bool HeightMap::loadRawFile(std::string fileName)
{
	bool loadFromFile = false;
	FILE* file = nullptr;

	// Opens the file in Read/Binary mode.
	file = fopen(fileName.data(), "rb");

	// Check if file was found and could open it
	if (file != nullptr)
	{
		// Read the .raw file into data array.
		fread(g_HeightMap, 1, mapSize, file);

		// Check for data error
		int result = ferror(file);
		result = errno;
		if (result) loadFromFile = false;
		else loadFromFile = true;

	}
	else
	{
		// Height map not found
		loadFromFile = false;
	}
	fclose(file);

	for (int _w = 0; _w < mapWidth; _w += quadSize)
	{
		for (int _h = 0; _h < mapHeight; _h += quadSize)
		{
			rgbColor = setVertexColor(_w, _h);
			
			Point2 temp;
			temp.ver = glm::vec3(_w, getHeight(_w, _h), _h);
			temp.col = glm::vec3(rgbColor, rgbColor, rgbColor);

			points.push_back(temp);
		}
	}

	return loadFromFile;
}
int HeightMap::getHeight(int _x, int _y)
{
	// Force x and y to cap at (mapSize - 1)
	int x = _x % mapWidth;
	int y = _y % mapHeight;

	// Check if empty
	if (!g_HeightMap)
	{
		// TODO: printf to console
		return 0;
	}

	// Treat the array like a 2D array (.raw format is a single array)
	return g_HeightMap[x + (y * mapHeight)];	// Index into our height array and return the height
}