#include <GL/glew.h>
#include <GLFW\glfw3.h>
#undef APIENTRY
#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "Graphics.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



#ifdef _DEBUG
extern "C"
{
	void APIENTRY openglCallbackFunction(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		void* userParam)
	{

		std::cout << "_____________________OpenGL_Callback____________________" << std::endl;
		std::cout << "MESSAGE: " << message << std::endl;
		std::cout << "TYPE: ";
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			std::cout << "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			std::cout << "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			std::cout << "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			std::cout << "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			std::cout << "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			std::cout << "OTHER";
			break;
		}
		std::cout << std::endl;
		std::cout << "ID: " << id << std::endl;
		std::cout << "SEVERITY: ";
		switch (severity){
		case GL_DEBUG_SEVERITY_LOW:
			std::cout << "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout << "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << "HIGH";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			std::cout << "NOTIFICATION";
			break;
		}
		std::cout << std::endl;
		std::cout << "--------------------------------------------------------" << std::endl;
	}
}
#endif

int main()
{
	GLFWwindow* window = nullptr;

	glfwSetErrorCallback(error_callback);

	// Inititiating GLFW library
	if (!glfwInit()) 
		exit(EXIT_FAILURE);

	// Sets hints for the next call to glfwCreateWindow - Keep these for future debugging
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);// This hint is ignored for windowed mode windows.

	// Creating window
	window = glfwCreateWindow(640, 480, "Title", NULL, NULL);

	// Failure check - Creating window
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(stderr, "GLFW Window initialized\n");
	}

	// Bind key-inputs
	glfwSetKeyCallback(window, key_callback);

	// Fetch OpenGL version - Makes sure correct version is loaded from GLEW
	glfwMakeContextCurrent(window);
	// Inititiating GLEW library
	glewExperimental = true;
	GLenum err = glewInit();

	// Failure check - Inititiating GLEW
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	else
	{
		fprintf(stderr, "GLEW initialized\n");
	}

#ifdef _DEBUG
	if (glDebugMessageCallback){
		std::cout << "Register OpenGL debug callback " << std::endl;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			&unusedIds,
			true);
	}
	else
	{
		std::cout << "glDebugMessageCallback not available" << std::endl;
	}
#endif

	// VSync
	glfwSwapInterval(1);

	////////////////////////////////////////////////////////////
	Graphics ge = Graphics();
	
	Camera cam = Camera(); 
	cam.SetPosition(glm::vec3(0.0f, 0, 2.0f));
	cam.UpdateProjectionMatrix();
	ge.setCamera(&cam);
	
	Point triangleData[3] =
	{
		glm::vec3(0.0f, 0.1f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.1f, -0.1f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	};

	Point triangleData2[3] =
	{
		glm::vec3(0.0f, 0.1f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.1f, -0.1f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	};
	
	Point squareData[4] =
	{
		glm::vec3(0.1f, 0.1f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(-0.1f, 0.1f, 0.0f),
		glm::vec2(1.0f, 0.0f),	
		glm::vec3(0.0f, 1.0f, 0.0f),	

		glm::vec3(-0.1f, -0.1f, 0.0f),
		glm::vec2(1.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f)	
	};

	std::vector<Point> vertexData = std::vector<Point>();

	vertexData.push_back(triangleData[0]);
	vertexData.push_back(triangleData[1]);
	vertexData.push_back(triangleData[2]);
	MeshObject tm = MeshObject(vertexData);

	vertexData.clear();
	vertexData.push_back(triangleData2[0]);
	vertexData.push_back(triangleData2[1]);
	vertexData.push_back(triangleData2[2]);
	MeshObject tm2 = MeshObject(vertexData);

	vertexData.clear();
	vertexData.push_back(squareData[0]);
	vertexData.push_back(squareData[1]);
	vertexData.push_back(squareData[2]);
	vertexData.push_back(squareData[3]);
	MeshObject sm = MeshObject(vertexData);

	// Square
	MeshHolder square = MeshHolder(&sm);
	square.SetRotation(glm::rotate(mat4(1.f), 45.f, vec3(0.f, 0.0f, 1.f)));
	square.SetTranslation(glm::translate(mat4(1.0f), vec3(0.5f, 0.0f, 0.0f)));

	// Square 2
	MeshHolder square2 = MeshHolder(&sm);
	square2.SetRotation(glm::rotate(mat4(1.f), 00.f, vec3(0.f, 0.0f, 1.f)));
	square2.SetTranslation(glm::translate(mat4(1.0f), vec3(-0.5f, 0.0f, 0.0f)));


	// Triangle
	MeshHolder triangle1 = MeshHolder(&tm2);
	triangle1.SetRotation(glm::rotate(mat4(1.f), 90.f, vec3(0.f, 0.0f, 1.f)));
	triangle1.SetTranslation(glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)));


	// Triangle 2
	MeshHolder triangle2 = MeshHolder(&tm2);
	triangle2.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	triangle2.SetTranslation(glm::translate(mat4(1.0f), vec3(0.0f, 0.5f, 0.0f)));


	// Triangle 3
	MeshHolder triangle3 = MeshHolder(&tm);
	triangle3.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	triangle3.SetTranslation(glm::translate(mat4(1.0f), vec3(0.0f, -0.5f, 0.0f)));

	std::vector<MeshObject*> meshes;
	meshes.push_back(&sm);
	meshes.push_back(&tm);
	meshes.push_back(&tm2);
	ge.GenerateBuffer(meshes);

	int width = 0, height = 0;
	////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window))
	{
		int newWidth, newHeight;
		glfwGetFramebufferSize(window, &newWidth, &newHeight);

		if (width != newWidth || height != newHeight)
		{
			width = newWidth;
			height = newHeight;
			glViewport(0, 0, width, height);

			/* TODO: Prompt update for cameras and such using projectionmatrices */
			cam.SetWidth((float)width);
			cam.SetHeight((float)height);	
			cam.UpdateProjectionMatrix();
		}

		/* TODO: Calling ge.Update() and ge.Render() */
		
		ge.PrepareRender();
		ge.Render(&square);
		ge.Render(&square2);
		ge.Render(&triangle1);
		ge.Render(&triangle2);
		ge.Render(&triangle3);

		glfwSwapBuffers(window);
		glfwPollEvents();// Processes all pending events
	}
	////////////////////////////////////////////////////////////
	
	glfwDestroyWindow(window);
	glfwTerminate();

	//system("pause");// Remove when main loop is working or save it to read the console's output before exit.
	return 0;
}

static void error_callback(int err, const char* d)
{
	fputs(d, stderr);
}
static void key_callback(GLFWwindow* w, int k, int scancode, int a, int mods)
{
	if (k == GLFW_KEY_ESCAPE && a == GLFW_PRESS)
		glfwSetWindowShouldClose(w, GL_TRUE);
}