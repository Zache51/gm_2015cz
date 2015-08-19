#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <gl/glew.h>
#include <GLFW\glfw3.h>

#include "Camera.hpp"
#include "MeshHolder.hpp"
#include "MeshObject.hpp"
#include <glm/gtc/quaternion.hpp>

// temp
#include <string>
#define SHADER_FOLDER "Shaders/"
//

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

using namespace glm;

class Graphics
{
private:
	Camera* localCamera;

	GLuint gVertexBuffer = 0;
	GLuint gIndexBuffer = 0;

	GLuint gVertexAttribute1 = 0;
	
	GLuint gShaderProgram = 0;

	GLint projectionviewworldMatrixUniformLocation;

	void createShaderStep(const char* filename, GLuint& shader);
	void linkProgram(std::vector<GLuint> shaders, GLuint& program);
	std::string readShader(const char *filePath);

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