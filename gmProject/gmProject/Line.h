#pragma once

#include "MeshBase.h"

class Line : public MeshBase
{
private:
	std::vector<Point_HeightMap> points;
	glm::vec3 color;

public:
	Line();
	~Line();

	void AddPoint(glm::vec3 position);
	void ClearVector();

	void* GetPointsData();
	GLuint GetFloatAmount() const;
};
