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
		
		uniform mat4 rotationMatrix;

		out vec3 color;
		
		void main () {
			color = vertex_color;
			gl_Position = rotationMatrix * vec4 (vertex_position, 1.0);
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

void Graphics::GenerateBuffer( MeshHolder* mh )
{

}
void Graphics::Update( MeshHolder* mh )
{

}
void Graphics::Render( MeshHolder* mh )
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//mat4 rotationMatrix = glm::rotate(mat4(1.f), 30.f, vec3(1.f, .0f, 0.f));

	//float alpha = (3.14 / 180) * 45;

	//mat4 rotationMatrix = mat4(
	//	glm::cos(alpha), 0.0, -glm::sin(alpha), 0.0,
	//	0.0, 1.0, 0.0, 0.0,
	//	glm::sin(alpha), 0.0, glm::cos(alpha), 0.0,
	//	0.0, 0.0, 0.0, 1.0
	//	);
	//	
	//GLint rotationMatrixUniformLocation = glGetUniformLocation(gShaderProgram, "rotationMatrix");
	//glUniformMatrix4fv(rotationMatrixUniformLocation, 1, GL_FALSE, &(GLfloat)rotationMatrix[0][0]);

	//mat4 PMatrix = perspective(3.14 * 0.45f, 640.0f / 480.0f, 0.5f, 20.0f);

	glUseProgram(gShaderProgram);
	glBindVertexArray(gVertexAttribute1);

	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	
}

void Graphics::setCamera( Camera* c )
{
	localCamera = c;
}