#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <gl/glew.h>
#include <gl/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Camera.hpp"
#include "MeshHolder.hpp"
#include "MeshObject.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

using namespace glm;

class Graphics
{
private:
	Camera* localCamera;

	GLuint gVertexBuffer = 0;
	GLuint gIndexBuffer = 0;

	GLuint gVertexAttribute1 = 0;
	GLuint gVertexAttribute2 = 0;
	
	GLuint gShaderProgram = 0;

	GLint projectionviewworldMatrixUniformLocation;

	void readShader( const char* path );
	void createProgram();
	void generateShaders();
public:
	Graphics();
	virtual ~Graphics();

	void GenerateBuffer(std::vector<MeshObject*> meshes);
	void PrepareRender();
	void Render( MeshHolder* mh );

	void SetCamera( Camera* c );
};

#endif