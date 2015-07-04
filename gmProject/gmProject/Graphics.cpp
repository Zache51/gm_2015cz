#include "Graphics.hpp"

Graphics::Graphics()
{
	// Code is temporary

	generateShaders();

	/*********************Create triangle data********************/
	struct TriangleVertex
	{
		float x, y, z;
		float r, g, b;
	};
	TriangleVertex tV1[3] =
	{
		0.0f, 0.1f, 0.0f,	//v0 pos
		1.0f, 0.0f, 0.0f,	//v0 color

		0.1f, -0.1f, 0.0f,	//v1
		1.0f, 0.0f, 0.0f,	//v1 color

		-0.1f, -0.1f, 0.0f, //v2
		1.0f, 0.0f, 0.0f	//v2 color
	};
	TriangleVertex tV2[4] =
	{
		0.1f, 0.1f, 0.0f,	//v0 pos
		0.0f, 1.0f, 0.0f,	//v0 color

		0.1f, -0.1f, 0.0f,	//v1
		0.0f, 1.0f, 0.0f,	//v1 color

		-0.1f, 0.1f, 0.0f,	//v2
		0.0f, 1.0f, 0.0f,	//v2 color

		-0.1f, -0.1f, 0.0f, //v3
		0.0f, 1.0f, 0.0f	//v3 color
	};

	//create buffer and set data
	glGenBuffers(1, &gVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tV1) + sizeof(tV2), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tV1), tV1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tV1), sizeof(tV2), tV2);

	//define vertex data layout
	glGenVertexArrays(1, &gVertexAttribute1);
	glGenVertexArrays(1, &gVertexAttribute2);

	GLuint vertexPos = glGetAttribLocation(gShaderProgram, "vertex_position");
	GLuint vertexColor = glGetAttribLocation(gShaderProgram, "vertex_color");

	glBindVertexArray(gVertexAttribute1);
	glEnableVertexAttribArray(0); //the vertex attribute object will remember its enabled attributes
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(0));
	glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(float) * 3));

	glBindVertexArray(gVertexAttribute2);
	glEnableVertexAttribArray(0); //the vertex attribute object will remember its enabled attributes
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(tV1)));
	glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(tV1) + sizeof(float) * 3));
	
	
	/*************************************************************/
	/*******************Define uniform locations******************/
	
	
	worldMatrixUniformLocation = glGetUniformLocation(gShaderProgram, "worldMatrix");
	viewworldMatrixUniformLocation = glGetUniformLocation(gShaderProgram, "VWMatrix");
	
	
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
		layout(location = 1) in vec3 vertex_color;
		
		uniform mat4 worldMatrix;
		uniform mat4 VWMatrix;

		out vec3 color;
		
		void main () {
			color = vertex_color;
			gl_Position = VWMatrix * vec4 (vertex_position, 1.0);
		}
	)";

	const char* fragment_shader = R"(
		#version 400
		in vec3 color;
		out vec4 fragment_color;
		void main () {
			fragment_color = vec4 (color, 1.0);
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

}
void Graphics::PrepareRender()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(gVertexAttribute1);

	glUseProgram(gShaderProgram);
}

void Graphics::Render( MeshHolder* mh )
{
	mat4 vwMatrix = viewMatrix * mh->GetWorld();

	glUniformMatrix4fv(worldMatrixUniformLocation, 1, GL_FALSE, &(GLfloat)mh->GetWorld()[0][0]);
	glUniformMatrix4fv(viewworldMatrixUniformLocation, 1, GL_FALSE, &(GLfloat)vwMatrix[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, mh->offset, mh->numberOfIndices);
	
}

void Graphics::setCamera( Camera* c )
{
	localCamera = c;
}