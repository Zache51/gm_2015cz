#include "Graphics.hpp"
#include "Physics.h"

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

// Meshes
MeshObject m;
MeshObject s_mesh;
MeshObject target_mesh;

// World objects
MeshHolder mustang;
MeshHolder mustang2;
MeshHolder mustang3;
MeshHolder mustang4;
MeshHolder mustangHigh;
MeshHolder ground;
MeshHolder target;
MeshHolder target2;
Line EndOfLine = Line();

// Cameras
std::vector<Camera> cameras;

void InitCameras();
void InitMeshes(Graphics* ge);
void MoveCamera(Camera* cam, GLFWwindow* window);
void RotateCamera(Camera* cam, GLFWwindow* window);
void UpdateProjections(GLFWwindow* window);

void Collision(vec3* point, MeshHolder* plane);

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

	// Engine classes
	Graphics ge = Graphics();
	Physics ph = Physics();

	InitMeshes(&ge);

	InitCameras();

	int cameraIndex = 1;
	ge.SetCamera(&cameras[cameraIndex]);
	
	fpsCounter fpsC;

	////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window))
	{
		std::stringstream ss;
		ss << fpsC.get();

		glfwSetWindowTitle(window, ss.str().c_str());

		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		{
			cameraIndex = 0;
			ge.SetCamera(&cameras[cameraIndex]);
		}
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		{
			cameraIndex = 1;
			ge.SetCamera(&cameras[cameraIndex]);
		}

		UpdateProjections(window);
		RotateCamera(&cameras[cameraIndex], window);
		MoveCamera(&cameras[cameraIndex], window);
		
		
		if (fpsC.deltaTime() > 0)
		{
			//ph.move(&mustangHigh, &fpsC);
			ph.move(&EndOfLine, &fpsC);
		}

		Collision(&EndOfLine.GetLastPoint(), &target);
		Collision(&EndOfLine.GetLastPoint(), &target2);

		ge.PrepareRender();
		ge.Render(&mustang);
		ge.Render(&mustang2);
		ge.Render(&mustang3);
		ge.Render(&mustang4);
		ge.Render(&mustangHigh);

		ge.Render(&ground);
		ge.Render(&target);
		ge.Render(&target2);

		ge.Render(&EndOfLine);

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



void InitCameras()
{
	Camera cam = Camera();
	cam.SetPosition(-glm::vec3(0.0f, 2.0f, 20.0f));
	cameras.push_back(cam);

	Camera fixedCam = Camera();
	fixedCam.SetPosition(-glm::vec3(0.0f, 80.0f, 20.0f));
	cameras.push_back(fixedCam);
}

void InitMeshes(Graphics* ge)
{
	fprintf(stdout, "\n");
	fprintf(stdout, "------------- Loading Meshes -------------\n");
	m = MeshObject("mustang.obj", 1);
	s_mesh = MeshObject("Square.obj", 7);
	target_mesh = MeshObject("Square_wall.obj", 1);
	fprintf(stdout, "------------------------------------------\n");

	// P-51 Mustang
	mustang = MeshHolder(&m);
	mustang.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustang.SetPosition(vec3(0.0f, 0.0f, 0.0f));

	mustang2 = MeshHolder(&m);
	mustang2.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustang2.SetPosition(vec3(0.0f, 0.0f, 20.0f));

	mustang3 = MeshHolder(&m);
	mustang3.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustang3.SetPosition(vec3(20.0f, 0.0f, 20.0f));

	mustang4 = MeshHolder(&m);
	mustang4.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustang4.SetPosition(vec3(20.0f, 0.0f, 0.0f));

	mustangHigh = MeshHolder(&m);
	mustangHigh.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	mustangHigh.SetPosition(vec3(60.0f, 60.0f, 0.0f));

	ground = MeshHolder(&s_mesh);
	ground.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	ground.SetPosition(vec3(10.0f, -1.5f, 10.0f));

	target = MeshHolder(&target_mesh);
	target.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	target.SetPosition(vec3(60.0f, 60.f, 100.0f));
	target.ChangeTexture(1);

	target2 = MeshHolder(&target_mesh);
	target2.SetRotation(glm::rotate(mat4(1.f), 0.f, vec3(0.f, 0.0f, 1.f)));
	target2.SetPosition(vec3(60.0f, 40.f, 100.0f));
	target2.ChangeTexture(1);

	std::vector<MeshObject*> meshes;
	meshes.push_back(&m);
	meshes.push_back(&s_mesh);
	meshes.push_back(&target_mesh);

	EndOfLine.AddPoint(vec3(60.0f, 60.0f, 0.0f));

	ge->GenerateBuffer(meshes);
	ge->GenerateLineBuffer();
}

void MoveCamera(Camera* cam, GLFWwindow* window)
{
	float cameraSpeed = 5;

	// Moved the camera with keyboard (WIP)
	glm::vec3 up = mat3(cam->GetRotationMatrix()) * glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 forward = mat3(cam->GetRotationMatrix()) * glm::vec3(0.0, 0.0, -1.0);
	glm::vec3 strafe = glm::cross(up, forward);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam->UpdatePosition(forward*glm::vec3(0.1f, 0, -0.1)*vec3(cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam->UpdatePosition(forward*glm::vec3(-0.1f, 0, 0.1)*vec3(cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam->UpdatePosition(strafe*glm::vec3(0.1f, 0, -0.1f)*vec3(cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam->UpdatePosition(strafe*glm::vec3(-0.1f, 0, 0.1)*vec3(cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		EndOfLine.AddPoint(-cam->GetPosition());
	}
}

float rx = 0, ry = 0, rz = 0;
void RotateCamera(Camera* cam, GLFWwindow* window)
{
	// Rotates the camera with the mouse (WIP)
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (xpos != rx && ypos != ry)
	{
		ry += (float)(xpos - ry);
		rx += (float)(ypos - rx);

		glm::quat quatx = glm::angleAxis(rx / 150.0f, glm::vec3(1, 0, 0));
		glm::quat quaty = glm::angleAxis(ry / 150.0f, glm::vec3(0, 1, 0));

		mat4 rotation = mat4(glm::mat3_cast(glm::cross(quatx, quaty)));

		cam->SetRotationMatrix(rotation);
	}
}

int width = 0, height = 0;
void UpdateProjections(GLFWwindow* window)
{
	int newWidth, newHeight;
	glfwGetFramebufferSize(window, &newWidth, &newHeight);

	if (width != newWidth || height != newHeight)
	{
		width = newWidth;
		height = newHeight;
		glViewport(0, 0, width, height);

		for (int i = 0; i < cameras.size(); i++)
		{
			cameras.at(i).SetScreenSize((float)width, (float)height);
		}
	}
}

vec3 posBefore = vec3(-100);
void Collision(vec3* point, MeshHolder* plane)
{
	vec3 planePos = plane->GetPosition();

	bool collide = false;

	if (posBefore.z < planePos.z)
	{
		if (posBefore.x > planePos.x - 10 && posBefore.x < planePos.x + 10)
		{
			if (posBefore.y > planePos.y - 10 && posBefore.y < planePos.y + 10)
			{
				collide = true;
			}
		}
	}

	if (collide)
	{
		if (point->z > planePos.z)
		{
			if (point->x > planePos.x - 10 && point->x < planePos.x + 10)
			{
				if (point->y > planePos.y - 10 && point->y < planePos.y + 10)
				{
					plane->ChangeTexture(0);
				}
			}
		}
	}


	posBefore = *point;
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