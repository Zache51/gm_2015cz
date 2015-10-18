#include "Line.h"


Line::Line()
{
	color = glm::vec3(1.0f, 0.8f, 0.0f);
	points = std::vector<Point_HeightMap>();
}

Line::~Line(){}

void Line::AddPoint(glm::vec3 position)
{
	// Inte tillräckligt bra. Måste fixa raka sträck

	points.push_back(Point_HeightMap(position, color));

	indicies.push_back(numberOfPoints);

	numberOfPoints++;
}

void Line::ClearVector()
{
	points.clear();
	numberOfPoints = 0;
}


void* Line::GetPointsData()
{
	return points.data();
}
GLuint Line::GetFloatAmount() const
{
	return numberOfPoints * sizeof(Point_HeightMap);
}