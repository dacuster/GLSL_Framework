#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL_Window.h>

GL_Window::GL_Window()
{
	mWidth = 800;
	mHeight = 600;

	initializeKeys();
}

GL_Window::GL_Window(GLint _width, GLint _height)
{
	mWidth = _width;
	mHeight = _height;

	initializeKeys();
}

GL_Window::~GL_Window()
{
	// Destroy the window.
	glfwDestroyWindow(mpWindow);

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
	mpWindow = glfwCreateWindow(mWidth, mHeight, "Test Window", NULL, NULL);

	// Window was not created.
	if (!mpWindow)
	{
		// Notify the user of failed window creation.
		printf("GLFW window creation failed!");

		// Terminate GLFW.
		glfwTerminate();

		// Return the error code.
		return 1;
	}

	// Get Buffer size information.
	glfwGetFramebufferSize(mpWindow, &mBufferWidth, &mBufferHeight);

	// Set the context for GLEW to use. Can use multiple windows.
	glfwMakeContextCurrent(mpWindow);

	// Handle event callbacks.
	createCallbacks();

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
		glfwDestroyWindow(mpWindow);

		// Terminate GLFW.
		glfwTerminate();

		// Return error code.
		return 1;
	}

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	// Setup viewport.
	glViewport(0, 0, mBufferWidth, mBufferHeight);

	// Get a pointer for event handling.
	glfwSetWindowUserPointer(mpWindow, this);

	// Initialized.
	return 0;
}

GLfloat GL_Window::getMouseChangeX()
{
	GLfloat change = mChangeMouseX;

	mChangeMouseX = 0.0f;

	return change;
}

GLfloat GL_Window::getMouseChangeY()
{
	GLfloat change = mChangeMouseY;

	mChangeMouseY = 0.0f;

	return change;
}

void GL_Window::createCallbacks()
{
	glfwSetKeyCallback(mpWindow, handleKeys);
	glfwSetCursorPosCallback(mpWindow, handleMouse);
}

void GL_Window::handleKeys(GLFWwindow* _pWindow, int _key, int _code, int _action, int _mode)
{
	GL_Window* pWindow = static_cast<GL_Window*>(glfwGetWindowUserPointer(_pWindow));

	// Check for escape key press.
	if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_pWindow, GL_TRUE);
	}

	// Make sure key is within the ASCII range.
	if (_key >= 0 && _key < 1024)
	{
		if (_action == GLFW_PRESS)
		{
			pWindow->mKeys[_key] = true;
		}
		else if (_action == GLFW_RELEASE)
		{
			pWindow->mKeys[_key] = false;
		}
	}
}

void GL_Window::handleMouse(GLFWwindow* _pWindow, double _xPosition, double _yPosition)
{
	GL_Window* pWindow = static_cast<GL_Window*>(glfwGetWindowUserPointer(_pWindow));

	if (pWindow->mMouseFirstMoved)
	{
		// Set the mouse coordinates for the initial frame.
		pWindow->mLastMouseX = (GLfloat)_xPosition;
		pWindow->mLastMouseY = (GLfloat)_yPosition;

		// Mouse moved on the screen already.
		pWindow->mMouseFirstMoved = false;
	}

	// Calculate the change in the mouse position.
	pWindow->mChangeMouseX = (GLfloat)_xPosition - pWindow->mLastMouseX;
	pWindow->mChangeMouseY = pWindow->mLastMouseX - (GLfloat)_yPosition;

	// Set the mouse coordinates for the next from.
	pWindow->mLastMouseX = (GLfloat)_xPosition;
	pWindow->mLastMouseY = (GLfloat)_yPosition;
}

void GL_Window::initializeKeys()
{
	for (size_t counter = 0; counter < 1024; counter++)
	{
		mKeys[counter] = false;
	}
}
