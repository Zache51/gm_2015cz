#ifndef MESHHOLDER_HPP
#define MESHHOLDER_HPP

#include "Movable.h"

#include "MeshObject.hpp"

using namespace glm;

class MeshHolder : public Movable
{
private:
	MeshObject* mesh;
	mat4 rotationMatrix = glm::rotate(mat4(1.f), 45.f, vec3(0.f, 0.0f, 1.f));

public:
	MeshHolder(){};
	MeshHolder(MeshObject* mesh);
	virtual ~MeshHolder(){};

	void SetRotation(mat4 rotation);

	mat4 GetWorld() const;
	MeshObject* GetMesh() const;
	
};

#endif