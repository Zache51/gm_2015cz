#include "Line.h"


Line::Line()
{
	color = glm::vec3(1.0f, 0.8f, 0.0f);

	points = std::vector<Point_HeightMap>();
	numberOfPoints = 0;

	indicies = std::vector<GLuint>();
	numberOfIndicies = 0;
}

Line::~Line(){}

void Line::AddPoint(glm::vec3 position)
{
	// Inte tillräckligt bra. Måste fixa raka sträck

	points.push_back(Point_HeightMap(position, color));

	indicies.push_back(numberOfPoints);

	numberOfPoints++;
	numberOfIndicies++;
}

glm::vec3 Line::GetLastPoint() const
{
	return points.back().ver;
}

void Line::ClearVector()
{
	points.clear();
	numberOfPoints = 0;

	indicies.clear();
	numberOfIndicies = 0;
}


void* Line::GetPointsData()
{
	return points.data();
}
GLuint Line::GetFloatAmount() const
{
	return numberOfPoints * sizeof(Point_HeightMap);
}