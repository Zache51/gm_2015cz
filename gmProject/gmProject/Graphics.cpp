#include "Graphics.hpp"

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

void Graphics::generateShaders()
{
	const char* vertex_shader = R"(
		#version 400
		layout(location = 0) in vec3 vertex_position;
		layout(location = 1) in vec2 texture_normal;
		layout(location = 2) in vec3 vertex_normal;
		
		out vec2 texture_normal_VS;

		uniform mat4 PVWMatrix;
		
		void main () {
			texture_normal_VS = texture_normal;
			gl_Position = PVWMatrix * vec4 (vertex_position, 1.0);
		}
	)";

	const char* fragment_shader = R"(
		#version 400
		in vec2 texture_normal_VS;
		uniform sampler2D texSampler;
		out vec4 fragment_color;
		void main () {
			vec4 mySample = texture(texSampler, vec2(texture_normal_VS.s, 1-texture_normal_VS.t));
			fragment_color = vec4( mySample.rgb, 1.0 );
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

	//link shader program (connect vs and fs)
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