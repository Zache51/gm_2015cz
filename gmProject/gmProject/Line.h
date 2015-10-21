#pragma once

#include "MeshBase.h"

class Line : public MeshBase
{
private:
	std::vector<Point_HeightMap> points;
	glm::vec3 color;

public:
	Line();
	Line(glm::vec3 color);
	~Line();

	void AddPoint(glm::vec3 position);
	glm::vec3 GetLastPoint() const;
	glm::vec3 GetSecondLastPoint() const;
	void ClearVector();

	void ChangeColor(glm::vec3 color);

	void* GetPointsData();
	GLuint GetFloatAmount() const;
};

