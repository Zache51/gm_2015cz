#include "Graphics.hpp"

#undef APIENTRY
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "FPSCounter.hpp"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#define PI 3.14f

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
	cam.SetPosition(glm::vec3(4.0f, 2.0f, 10.0f));
	ge.SetCamera(&cam);



	fprintf(stdout, "\n");
	fprintf(stdout, "------------- Loading Meshes -------------\n");
	//MeshObject tm = MeshObject("Triangle.obj");
	//MeshObject sm = MeshObject("Square.obj");
	MeshObject m = MeshObject("mustang.obj");
	fprintf(stdout, "------------------------------------------\n");

	// P-51 Mustang
	MeshHolder mustang = MeshHolder(&m);
	mustang.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustang.SetTranslation(glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)));

	MeshHolder mustang2 = MeshHolder(&m);
	mustang2.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustang2.SetTranslation(glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 20.0f)));

	MeshHolder mustang3 = MeshHolder(&m);
	mustang3.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustang3.SetTranslation(glm::translate(mat4(1.0f), vec3(20.0f, 0.0f, 20.0f)));

	MeshHolder mustang4 = MeshHolder(&m);
	mustang4.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustang4.SetTranslation(glm::translate(mat4(1.0f), vec3(20.0f, 0.0f, 0.0f)));

	MeshHolder mustangHigh = MeshHolder(&m);
	mustangHigh.SetRotation(glm::rotate(mat4(1.f), 3.14f, vec3(0.f, 0.0f, 1.f)));
	mustangHigh.SetTranslation(glm::translate(mat4(1.0f), vec3(0.0f, 60.0f, 0.0f)));

	std::vector<MeshObject*> meshes;
	meshes.push_back(&m);
	
	ge.GenerateBuffer(meshes);

	HeightMap heightmap = HeightMap("terrain.raw", &cam);
	ge.GenerateHeightMapBuffer(&heightmap);

	int width = 0, height = 0;
	fpsCounter fpsC;
	////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window))
	{
		std::stringstream ss;
		ss << fpsC.get() /*<< "   Height map draw count: " << heightmap.GetRenderCount()*/;

		glfwSetWindowTitle(window, ss.str().c_str());

		int newWidth, newHeight;
		glfwGetFramebufferSize(window, &newWidth, &newHeight);

		if (width != newWidth || height != newHeight)
		{
			width = newWidth;
			height = newHeight;
			glViewport(0, 0, width, height);

			cam.SetScreenSize((float)width, (float)height);
		}


		// Rotates the camera with the mouse (WIP)
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (xpos != cam.rx && ypos != cam.ry)
		{
			cam.ry += (float)(xpos - cam.ry) ;
			cam.rx += (float)(ypos - cam.rx) ;

			glm::quat quatx = glm::angleAxis(cam.rx / 150.0f, glm::vec3(1, 0, 0));
			glm::quat quaty = glm::angleAxis(cam.ry / 150.0f, glm::vec3(0, 1, 0));

			mat4 rotation = mat4(glm::mat3_cast(glm::cross(quatx, quaty)));

			cam.SetRotationMatrix(rotation);
		}

		// Moved the camera with keyboard (WIP)
		glm::vec3 up = mat3(cam.GetRotationMatrix()) * glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 forward = mat3(cam.GetRotationMatrix()) * glm::vec3(0.0, 0.0, -1.0);
		glm::vec3 strafe = glm::cross(up, forward);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cam.UpdateTranslation(forward*glm::vec3(0.1f, 0, -0.1)*vec3(5));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cam.UpdateTranslation(forward*glm::vec3(-0.1f, 0, 0.1)*vec3(5));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cam.UpdateTranslation(strafe*glm::vec3(0.1f, 0, -0.1f)*vec3(5));
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cam.UpdateTranslation(strafe*glm::vec3(-0.1f, 0, 0.1)*vec3(5));
		}

		

		ge.PrepareRender();
		ge.Render(&mustang);
		ge.Render(&mustang2);
		ge.Render(&mustang3);
		ge.Render(&mustang4);
		ge.Render(&mustangHigh);

		ge.Render(&heightmap);

		fpsC.tick();
		Sleep(1000 / 120);

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