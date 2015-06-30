#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "Camera.hpp"
#include "MeshHolder.hpp"

class Graphics
{
private:
	Camera* localCamera;

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