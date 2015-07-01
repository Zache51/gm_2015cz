#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <gl/glew.h>
#include <gl/GL.h>

#include "Camera.hpp"
#include "MeshHolder.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class Graphics
{
private:
	Camera* localCamera;

	GLuint gVertexBuffer = 0;
	GLuint gVertexAttribute = 0;
	GLuint gShaderProgram = 0;

	void readShader( const char* path );
	void createProgram();
	void generateShaders();
public:
	Graphics();
	virtual ~Graphics();

	void GenerateBuffer( MeshHolder* mh );
	void Update( MeshHolder* mh );
	void Render( MeshHolder* mh );

	void setCamera( Camera* c );
};

#endif