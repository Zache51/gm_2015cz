#include "Line.h"


Line::Line()
{
	color = glm::vec3(1.0f, 0.8f, 0.0f);

	points = std::vector<Point_HeightMap>();
	numberOfPoints = 0;

	indicies = std::vector<GLuint>();
	numberOfIndicies = 0;
}

Line::Line(glm::vec3 color)
{
	this->color = color;

	points = std::vector<Point_HeightMap>();
	numberOfPoints = 0;

	indicies = std::vector<GLuint>();
	numberOfIndicies = 0;
}

Line::~Line(){}

void Line::AddPoint(glm::vec3 position)
{
	// Inte tillr�ckligt bra. M�ste fixa raka str�ck

	points.push_back(Point_HeightMap(position, color));

	indicies.push_back(numberOfPoints);

	numberOfPoints++;
	numberOfIndicies++;
}

glm::vec3 Line::GetLastPoint() const
{
	return points.back().ver;
}

glm::vec3 Line::GetSecondLastPoint() const
{
	if (numberOfPoints > 1) return points[numberOfPoints-2].ver;
	else return glm::vec3(-100);
}

void Line::ClearVector()
{
	points.clear();
	numberOfPoints = 0;

	indicies.clear();
	numberOfIndicies = 0;
}

void Line::ChangeColor(glm::vec3 color)
{
	this->color = color;
}

void* Line::GetPointsData()
{
	return points.data();
}
GLuint Line::GetFloatAmount() const
{
	return numberOfPoints * sizeof(Point_HeightMap);
}