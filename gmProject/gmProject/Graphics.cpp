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
void Graphics::createShaderStep(const char* filename, GLuint& shader, std::vector<GLuint>& shaders)
{
	std::string content;
	content = readShader(filename);
	const char* vertex_shader = content.c_str();
	glShaderSource(shader, 1, &vertex_shader, nullptr);
	glCompileShader(shader);

	// Save shader step together with the other shader steps
	shaders.push_back(shader);
}
void Graphics::linkProgram(std::vector<GLuint>& shaders, GLuint& program)
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

	// Clear the vector so it can be reused
	shaders.clear();
}

void Graphics::generateShaders()
{
	// Used to temporary store shader steps for a program, cleared in linkProgram(...)
	std::vector<GLuint> shaders;

	GLuint vs;
	GLuint gs;
	GLuint fs;
	
	// Create shader steps for obj
	vs = glCreateShader(GL_VERTEX_SHADER);
	createShaderStep("obj_vs.glsl", vs, shaders);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	createShaderStep("obj_fs.glsl", fs, shaders);

	// Link program for obj
	linkProgram(shaders, objProgram);

	// Create shader steps for height map
	vs = glCreateShader(GL_VERTEX_SHADER);
	createShaderStep("heightmap_vs.glsl", vs, shaders);
	gs = glCreateShader(GL_GEOMETRY_SHADER);
	createShaderStep("heightmap_gs.glsl", gs, shaders);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	createShaderStep("heightmap_fs.glsl", fs, shaders);

	// Link program for height map
	linkProgram(shaders, heightmapProgram);
}

/***********************************************************************/
/******************************* Public ********************************/


Graphics::Graphics()
{

	generateShaders();

	pvwMatrixUniformLocation = glGetUniformLocation(objProgram, "PVWMatrix");
	pvwMatrixUniformLocation2 = glGetUniformLocation(heightmapProgram, "PVWMatrix");

}
Graphics::~Graphics()
{
	glDeleteBuffers(1, &vbObj);
	glDeleteBuffers(1, &ibObj);
	glDeleteProgram(objProgram);
	glDeleteProgram(heightmapProgram);
}

void Graphics::GenerateBuffer(std::vector<MeshObject*> meshes)
{
	//create vertex and index buffer 
	glGenBuffers(1, &vbObj);
	glBindBuffer(GL_ARRAY_BUFFER, vbObj);
	glGenBuffers(1, &ibObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibObj);

	// Define the size of the buffers	
	GLuint floatAmount = 0;
	GLuint GLuintAmount = 0;
	for each (MeshObject* mesh in meshes)
	{
		floatAmount += mesh->GetFloatAmount();
		GLuintAmount += mesh->GetGLuintAmount();
	}
	glBufferData(GL_ARRAY_BUFFER, floatAmount, 0, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLuintAmount, 0, GL_DYNAMIC_DRAW);


	// Define size and offset of the different subdata in the buffers
	GLuint offsetVer = 0;
	GLuint offsetInd = 0;
	for each (MeshObject* mesh in meshes)
	{
		// Set offset for mesh
		GLuint add = offsetVer / mesh->GetSizeOfPoint();
		mesh->SetOffset(add);
		mesh->SetOffsetInd(offsetInd);
		
		glBufferSubData(GL_ARRAY_BUFFER,
			offsetVer,
			mesh->GetFloatAmount(),
			mesh->GetPointsData());
		
		mesh->AddIndicies(add);
		
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
			offsetInd,
			mesh->GetGLuintAmount(),
			mesh->GetIndiciesData());

		offsetVer += mesh->GetFloatAmount();
		offsetInd += mesh->GetGLuintAmount();

		mesh->FreeMemory();
	}

	// Define VAO
	glGenVertexArrays(1, &gVertexAttributeObj);
	glBindVertexArray(gVertexAttributeObj);
	glBindBuffer(GL_ARRAY_BUFFER, vbObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibObj);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	// Define vertex data layout	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, meshes[0]->GetSizeOfPoint(), BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, meshes[0]->GetSizeOfPoint(), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, meshes[0]->GetSizeOfPoint(), BUFFER_OFFSET(sizeof(float) * 5));
}

void Graphics::GenerateHeightMapBuffer(HeightMap* heightmap)
{
	//create vertex and index buffer 
	glGenBuffers(1, &vbHeightMap);
	glBindBuffer(GL_ARRAY_BUFFER, vbHeightMap);
	glGenBuffers(1, &ibHeightMap);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibHeightMap);

	// Define the size of the buffers	
	glBufferData(GL_ARRAY_BUFFER, heightmap->GetFloatAmount(), heightmap->GetPointsData(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, heightmap->GetGLuintAmount(), heightmap->GetIndiciesData(), GL_STATIC_DRAW);

	heightmap->FreeMemory();

	// Define VAO
	glGenVertexArrays(1, &gVertexAttributeheightMap);
	glBindVertexArray(gVertexAttributeheightMap);
	glBindBuffer(GL_ARRAY_BUFFER, vbHeightMap);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibHeightMap);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Define vertex data layout	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6, BUFFER_OFFSET(sizeof(float) * 3));
}

void Graphics::PrepareRender()
{
	glClearColor(0, 0, 0.08f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(objProgram);
}

void Graphics::Render( MeshHolder* mh )
{
	glBindVertexArray(gVertexAttributeObj);

	// Uniforms
	mat4 pvwMatrix = localCamera->GetPVMatrix() * mh->GetWorld();
	glUniformMatrix4fv(pvwMatrixUniformLocation, 1, GL_FALSE, &(GLfloat)pvwMatrix[0][0]);
	glBindTexture(GL_TEXTURE_2D, mh->GetMesh()->GetMtl().TextureID);

	// Render the mesh
	//glDrawArrays(GL_TRIANGLES, mh->mesh->GetOffset(), mh->mesh->GetPoints().size());
	glDrawElements(GL_TRIANGLES, mh->GetMesh()->GetNumberOfIndicies(), GL_UNSIGNED_INT, (void*)mh->GetMesh()->GetOffsetInd());
}

void Graphics::Render(HeightMap* hm)
{
	glUseProgram(heightmapProgram);

	glBindVertexArray(gVertexAttributeheightMap);

	// Uniforms
	mat4 pvwMatrix = localCamera->GetPVMatrix();
	glUniformMatrix4fv(pvwMatrixUniformLocation2, 1, GL_FALSE, &(GLfloat)pvwMatrix[0][0]);

	// Render the mesh
	glDrawArrays(GL_TRIANGLES, 0, hm->GetFloatAmount());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibHeightMap);

	// Render the mesh
	//hm->RenderHeightMap(localCamera);
}

void Graphics::SetCamera( Camera* c )
{
	localCamera = c;
}