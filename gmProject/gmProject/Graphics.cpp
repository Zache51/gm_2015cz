#include "Graphics.hpp"

#include <fstream>

std::string Graphics::readShader(const char *filePath)
{
	std::string content;
	std::ifstream fileStream(SHADER_FOLDER + (std::string)filePath, std::ios::in);

	if (!fileStream.is_open()) {
		fprintf(stderr, "Could not open file %s", filePath);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}
void Graphics::createShaderStep(const char* filename, GLuint& shader)
{
	std::string content;
	content = readShader(filename);
	const char* vertex_shader = content.c_str();
	glShaderSource(shader, 1, &vertex_shader, nullptr);
	glCompileShader(shader);
}
void Graphics::linkProgram(std::vector<GLuint> shaders, GLuint& program)
{
	//link shader program 
	program = glCreateProgram();
	for (size_t i = 0; i < shaders.size(); i++)
	{
		glAttachShader(program, shaders[i]);
	}
	glLinkProgram(program);

	//debug info regarding linking
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(program, 1024, &log_length, message);
		fprintf(stdout, message);
	}
}

void Graphics::generateShaders()
{
	std::vector<GLuint> shaders;
	// Create shader steps for obj
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	createShaderStep("vertex.glsl", vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	createShaderStep("fragment.glsl", fs);

	// Link program for obj
	shaders.push_back(vs);
	shaders.push_back(fs);
	linkProgram(shaders, gShaderProgram);
	shaders.clear();
}

/***********************************************************************/
/******************************* Public ********************************/


Graphics::Graphics()
{

	generateShaders();

	projectionviewworldMatrixUniformLocation = glGetUniformLocation(gShaderProgram, "PVWMatrix");

}
Graphics::~Graphics()
{
	glDeleteBuffers(1, &gVertexBuffer);
	glDeleteBuffers(1, &gIndexBuffer);
	glDeleteProgram(gShaderProgram);
}

void Graphics::GenerateBuffer(std::vector<MeshObject*> meshes)
{
	//create vertex and index buffer 
	glGenBuffers(1, &gVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);	
	glGenBuffers(1, &gIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

	// Define the size of the buffers	
	GLuint floatAmount = 0;
	GLuint GLuintAmount = 0;
	for each (MeshObject* mesh in meshes)
	{
		floatAmount += mesh->GetFloatAmount();
		GLuintAmount += mesh->GetGLuintAmount();
	}
	glBufferData(GL_ARRAY_BUFFER, floatAmount, 0, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLuintAmount, 0, GL_STATIC_DRAW);


	// Define size and offset of the different subdata in the buffers
	GLuint offsetVer = 0;
	GLuint offsetInd = 0;
	for each (MeshObject* mesh in meshes)
	{
		// Set offset for mesh
		GLuint add = offsetVer / sizeof(Point);
		mesh->SetOffset(add);
		mesh->SetOffsetInd(offsetInd);
		
		glBufferSubData(GL_ARRAY_BUFFER,
			offsetVer,
			mesh->GetFloatAmount(),
			mesh->GetPoints().data());
		
		mesh->AddIndicies(add);
		
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
			offsetInd,
			mesh->GetGLuintAmount(),
			mesh->GetIndicies().data());

		offsetVer += mesh->GetFloatAmount();
		offsetInd += mesh->GetGLuintAmount();
	}

	
	// Define vertex data layout	
	
	glGenVertexArrays(1, &gVertexAttribute1);
	glBindVertexArray(gVertexAttribute1);

	GLuint vertexPos = glGetAttribLocation(gShaderProgram, "vertex_position");
	GLuint textureNormal = glGetAttribLocation(gShaderProgram, "texture_normal");
	GLuint vertexNormal = glGetAttribLocation(gShaderProgram, "vertex_normal");
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Point), BUFFER_OFFSET(0));
	glVertexAttribPointer(textureNormal, 2, GL_FLOAT, GL_FALSE, sizeof(Point), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Point), BUFFER_OFFSET(sizeof(float) * 5));

}
void Graphics::PrepareRender()
{
	glClearColor(0, 0, 0.08f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(gShaderProgram);
}

void Graphics::Render( MeshHolder* mh )
{
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

	// Uniforms
	mat4 vwMatrix = localCamera->GetPVMatrix() * mh->GetWorld();
	glUniformMatrix4fv(projectionviewworldMatrixUniformLocation, 1, GL_FALSE, &(GLfloat)vwMatrix[0][0]);
	glBindTexture(GL_TEXTURE_2D, mh->GetMesh()->GetMtl().TextureID);

	// Render the mesh
	//glDrawArrays(GL_TRIANGLES, mh->mesh->GetOffset(), mh->mesh->GetPoints().size());
	glDrawElements(GL_TRIANGLES, mh->GetMesh()->GetNumberOfIndicies(), GL_UNSIGNED_INT, (void*)mh->GetMesh()->GetOffsetInd());
}

void Graphics::SetCamera( Camera* c )
{
	localCamera = c;
}