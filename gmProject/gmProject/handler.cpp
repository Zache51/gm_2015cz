#include <GL/glew.h>
//#include <GL/GL.h>// Not entirely sure this is required here

#include <GLFW\glfw3.h>
#undef APIENTRY

#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

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
	bool running = false;// For safe shutdown

	glfwSetErrorCallback(error_callback);

	// Inititiating GLFW library
	if (!glfwInit()) 
		exit(EXIT_FAILURE);

	// Sets hints for the next call to glfwCreateWindow - Keep these for future debugging
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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
		running = true;
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

	//int left, top, right, bottom;
	//glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
	//std::cout << "Left: " << left << ", Top: " << top << ", Right: " << right << ", Bottom: " << bottom << std::endl;
	//int xpos, ypos;
	//glfwGetWindowPos(window, &xpos, &ypos);
	//std::cout << "X: " << xpos << ", Y: " << ypos << std::endl;

	////////////////////////////////////////////////////////////
	running = false;
	// VSync
	glfwSwapInterval(1);
	while (running || !glfwWindowShouldClose(window))
	{

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		
		/* TODO - Update and Render function */

		glfwSwapBuffers(window);
		glfwPollEvents();// Processes all pending events
	}
	////////////////////////////////////////////////////////////
	
	glfwDestroyWindow(window);
	glfwTerminate();

	system("pause");// Remove when main loop is working or save it to read the console's output before exit.
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