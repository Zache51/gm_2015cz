#include "Grid.h"


Grid::Grid(int xAmount, int yAmount, MeshObject* mesh, MeshHolder* center)
{
	this->xAmount = xAmount;
	this->yAmount = yAmount;
	this->mesh = mesh;

	float space = 2.5; // Value above 2 makes space between quads
	float combine = space * mesh->GetScale();

	arr = new MeshHolder*[xAmount];
	for (int x = 0; x < xAmount; x++)
	{
		arr[x] = new MeshHolder[yAmount];
		for (int y = 0; y < yAmount; y++)
		{
			arr[x][y] = MeshHolder(mesh);
			arr[x][y].SetRotation(center->GetRotation());

			arr[x][y].SetPosition(vec3(combine*x*10.0f, -1.5f, combine*y*10.0f));

		}
	}
}

Grid::~Grid()
{
	for (int x = 0; x < xAmount; x++)
	{
		delete[] arr[x];
	}
	delete[] arr;
}

std::vector<MeshHolder*> Grid::GetMeshHolders()
{
	std::vector<MeshHolder*> meshHolders;

	for (int x = 0; x < xAmount; x++)
	{
		for (int y = 0; y < yAmount; y++)
		{
			meshHolders.push_back(&arr[x][y]);
		}
	}

	return meshHolders;
}
