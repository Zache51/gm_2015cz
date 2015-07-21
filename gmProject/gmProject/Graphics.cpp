#include "Graphics.hpp"

Graphics::Graphics()
{
	// Code is temporary

	generateShaders();

	
	/*************************************************************/
	/*******************Define uniform locations******************/
	
	projectionviewworldMatrixUniformLocation = glGetUniformLocation(gShaderProgram, "PVWMatrix");
	
	
	/*************************************************************/
}
Graphics::~Graphics()
{

}

void Graphics::generateShaders()
{
	const char* vertex_shader = R"(
		#version 400
		layout(location = 0) in vec3 vertex_position;
		layout(location = 1) in vec2 texture_normal;
		layout(location = 2) in vec3 vertex_normal;
		
		out vec2 color;

		uniform mat4 PVWMatrix;
		
		void main () {
			color = texture_normal;
			gl_Position = PVWMatrix * vec4 (vertex_position, 1.0);
		}
	)";

	const char* fragment_shader = R"(
		#version 400
		in vec2 color;
		out vec4 fragment_color;
		void main () {
			fragment_color = vec4 (color, 0.0, 1.0);
		}
	)";

	//create vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, nullptr);
	glCompileShader(vs);

	//debug info regarding vertex shader compilation
	GLint vertex_compiled;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(vs, 1024, &log_length, message);
	}

	//create fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, nullptr);
	glCompileShader(fs);

	//debuf info regarding fragment shader compilation
	GLint fragment_compiled;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &fragment_compiled);
	if (fragment_compiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(fs, 1024, &log_length, message);
	}

	//link shader program (connect vs and ps)
	gShaderProgram = glCreateProgram();
	glAttachShader(gShaderProgram, fs);
	glAttachShader(gShaderProgram, vs);
	glLinkProgram(gShaderProgram);

	//debug info regarding linking
	GLint linked;
	glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &linked);
	if (linked != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(gShaderProgram, 1024, &log_length, message);
	}
}

void Graphics::GenerateBuffer(std::vector<MeshObject*> meshes)
{
	//create buffer and set data	
	glGenBuffers(1, &gVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);	


	// Define the size of the buffer	
	GLuint floatAmount = 0;
	for each (MeshObject* mesh in meshes)
	{
		floatAmount += mesh->GetFloatAmount();
	}
	glBufferData(GL_ARRAY_BUFFER, floatAmount, 0, GL_STATIC_DRAW);


	// Define size and offset of the different subdata in the buffer	
	GLuint offset = 0;
	for each (MeshObject* mesh in meshes)
	{
		// Set offset for mesh
		mesh->SetOffset(offset / sizeof(Point));
		
		glBufferSubData(GL_ARRAY_BUFFER,
			offset,
			mesh->GetFloatAmount(),
			mesh->GetPoints().data());
		offset += mesh->GetFloatAmount();
	}


	//define vertex data layout	
	glGenVertexArrays(1, &gVertexAttribute1);
	glGenVertexArrays(1, &gVertexAttribute2);
	GLuint vertexPos = glGetAttribLocation(gShaderProgram, "vertex_position");
	GLuint textureNormal = glGetAttribLocation(gShaderProgram, "texture_normal");
	GLuint vertexNormal = glGetAttribLocation(gShaderProgram, "vertex_normal");
	glBindVertexArray(gVertexAttribute1);
	glEnableVertexAttribArray(0); //the vertex attribute object will remember its enabled attributes
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Point), BUFFER_OFFSET(0));
	glVertexAttribPointer(textureNormal, 2, GL_FLOAT, GL_FALSE, sizeof(Point), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Point), BUFFER_OFFSET(sizeof(float) * 5));
}
void Graphics::PrepareRender()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//glBindVertexArray(gVertexAttribute1);

	glUseProgram(gShaderProgram);
}

void Graphics::Render( MeshHolder* mh )
{
	mat4 vwMatrix = localCamera->GetPVMatrix() * mh->GetWorld();

	glUniformMatrix4fv(projectionviewworldMatrixUniformLocation, 1, GL_FALSE, &(GLfloat)vwMatrix[0][0]);

	glDrawArrays(GL_TRIANGLE_STRIP, mh->mesh->GetOffset(), mh->mesh->GetPoints().size());
}

void Graphics::setCamera( Camera* c )
{
	localCamera = c;
}