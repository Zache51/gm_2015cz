#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <gl/glew.h>
#include <GLFW\glfw3.h>

#include "Camera.hpp"
#include "HeightMap.h"
#include "MeshHolder.hpp"
#include "MeshObject.hpp"
#include "Line.h"
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

	GLuint vbObj = 0;
	GLuint ibObj = 0;
	GLuint gVertexAttributeObj = 0;

	GLuint vbHeightMap = 0;
	GLuint ibHeightMap = 0;
	GLuint gVertexAttributeheightMap = 0;

	GLuint vbLine;
	GLuint ibLine;
	GLuint gVertexAttributeLine;
	
	GLuint objProgram = 0;
	GLuint heightmapProgram = 0;

	GLint pvwMatrixUniformLocation;
	GLint pvwMatrixUniformLocation2;

	void createShaderStep(const char* filename, GLuint& shader, std::vector<GLuint>& shaders);
	void linkProgram(std::vector<GLuint>& shaders, GLuint& program);
	std::string readShader(const char *filePath);

	void generateShaders();
public:
	Graphics();
	virtual ~Graphics();

	void GenerateBuffer(std::vector<MeshObject*> meshes);
	void GenerateHeightMapBuffer(HeightMap* heightmap);
	void GenerateLineBuffer(std::vector<Line*> lines);

	void PrepareRender();
	void Render(MeshHolder* mh);
	void Render(HeightMap* hm);
	void Render(Line* l);

	void SetCamera( Camera* c );
};

#endif