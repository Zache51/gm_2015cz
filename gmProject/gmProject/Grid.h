#pragma once

#include "MeshObject.hpp"
#include "MeshHolder.hpp"

#include <vector> //temp?

class Grid
{
private :
	MeshObject* mesh;

	int xAmount;
	int yAmount;

	MeshHolder** arr;

public:
	Grid(int xAmount, int yAmount, MeshObject* mesh, MeshHolder* center);
	~Grid();

	std::vector<MeshHolder*> GetMeshHolders();
};

