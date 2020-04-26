// Windows libraries.
#include <stdio.h>

// GL libraries.
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimentions.
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

int main()
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
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	// Window was not created.
	if (!mainWindow)
	{
		// Notify the user of failed window creation.
		printf("GLFW window creation failed!");

		// Terminate GLFW.
		glfwTerminate();

		// Return the error code.
		return 1;
	}

	// Create buffer sizes.
	int bufferWidth;
	int bufferHeight;

	// Get Buffer size information.
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the context for GLEW to use. Can use multiple windows.
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features.
	glewExperimental = GL_TRUE;

	// Initialize GLEW.
	if (glewInit() != GLEW_OK)
	{
		// Notify user of failed initialization.
		printf("GLEW initialization failed!");

		// Destroy the window.
		glfwDestroyWindow(mainWindow);

		// Terminate GLFW.
		glfwTerminate();

		// Return error code.
		return 1;
	}

	// Setup viewport.
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Loop until window is closed.
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user input event.
		glfwPollEvents();

		// Clear the window to red.
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		// Clear the color buffer data.
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap to back buffer.
		glfwSwapBuffers(mainWindow);
	}

	// Return error code.
	return 0;
}