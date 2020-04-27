#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL_Window.h>

GL_Window::GL_Window()
{
	mWidth = 800;
	mHeight = 600;
}

GL_Window::GL_Window(GLint _width, GLint _height)
{
	mWidth = _width;
	mHeight = _height;
}

GL_Window::~GL_Window()
{
	// Destroy the window.
	glfwDestroyWindow(pWindow);

	// Terminate GLFW.
	glfwTerminate();
}

int GL_Window::initialize()
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		// Notify the user of failed initialization.
		printf("GLFW initialization failed!");

		// Terminate GLFW.
		glfwTerminate();

		// Return the error.
		return 1;
	}

	/***********************************
	** Setup GLFW window properties.  **
	***********************************/

	// OpenGL version.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No backward compatibility.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compatibility.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create new window.
	pWindow = glfwCreateWindow(mWidth, mHeight, "Test Window", NULL, NULL);

	// Window was not created.
	if (!pWindow)
	{
		// Notify the user of failed window creation.
		printf("GLFW window creation failed!");

		// Terminate GLFW.
		glfwTerminate();

		// Return the error code.
		return 1;
	}

	// Get Buffer size information.
	glfwGetFramebufferSize(pWindow, &mBufferWidth, &mBufferHeight);

	// Set the context for GLEW to use. Can use multiple windows.
	glfwMakeContextCurrent(pWindow);

	// Allow modern extension features.
	glewExperimental = GL_TRUE;

	// Initialize GLEW.
	GLenum error = glewInit();

	// Check for initialization error.
	if (error != GLEW_OK)
	{
		// Notify user of failed initialization.
		printf("Error: %s", glewGetErrorString(error));

		// Destroy the window.
		glfwDestroyWindow(pWindow);

		// Terminate GLFW.
		glfwTerminate();

		// Return error code.
		return 1;
	}

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	// Setup viewport.
	glViewport(0, 0, mBufferWidth, mBufferHeight);

	// Initialized.
	return 0;
}
