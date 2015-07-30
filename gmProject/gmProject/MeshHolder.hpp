#ifndef MESHHOLDER_HPP
#define MESHHOLDER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshObject.hpp"

using namespace glm;

class MeshHolder
{
private:
	MeshObject* mesh;
	mat4 rotationMatrix = glm::rotate(mat4(1.f), 45.f, vec3(0.f, 0.0f, 1.f));
	mat4 translationMatrix = glm::translate(mat4(1.0f), vec3(0.5f, 0.0f, 0.0f));

public:
	MeshHolder(MeshObject* mesh);
	virtual ~MeshHolder(){};

	void SetRotation(mat4 rotation);
	void SetTranslation(mat4 translation);

	mat4 GetWorld() const;
	MeshObject* GetMesh() const;
	
};

#endif