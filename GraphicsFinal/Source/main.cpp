// Windows libraries.
#include <stdio.h>
#include <string>

// GL libraries.
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Project libraries.
#include <ShaderUtility.h>

// Window dimentions.
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

// Vertex data.
GLuint VAO;
GLuint VBO;

const std::string vertexShaderFile = "resources\\vs\\shader.vert";
const std::string fragmentShaderFile = "resources\\fs\\shader.frag";

void CreateTriangle()
{
	GLfloat verticies[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	// Create a vertex array object.
	glGenVertexArrays(1, &VAO);
	
	// Bind the VAO.
	glBindVertexArray(VAO);

		// Create a vertex buffer object.
		glGenBuffers(1, &VBO);
		
		// Bind the VBO.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

			// Pass verticies into buffer. Not going to be edited (GL_STATIC_DRAW)
			glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		
		// Unbind the VBO.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Unbind the VAO
	glBindVertexArray(0);
}

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

	// Create a triangle.
	CreateTriangle();

	// Create a shader loading utility.
	ShaderUtility shaderUtility;

	// Initialize the utility.
	shaderUtility.initialize();

	// Load in the vertex and fragment shaders.
	shaderUtility.loadShader(GL_FRAGMENT_SHADER, fragmentShaderFile);
	shaderUtility.loadShader(GL_VERTEX_SHADER, vertexShaderFile);

	// Link the shader program.
	shaderUtility.linkProgram();

	// Validate the shader program.
	shaderUtility.validateProgram();

	// Loop until window is closed.
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user input event.
		glfwPollEvents();

		// Clear the window to black.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Clear the color buffer data.
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shader program.
		shaderUtility.useProgram();

			// Use this VAO for the shader.
			glBindVertexArray(VAO);
			
				// Draw the varticies in the VAO.
				glDrawArrays(GL_TRIANGLES, 0, 3);

			// Unbind the VAO.
			glBindVertexArray(0);

		// Stop using the shader program.
		shaderUtility.stopProgram();

		// Swap to back buffer.
		glfwSwapBuffers(mainWindow);
	}

	// Delete the shaders.
	shaderUtility.deleteProgram();

	// Return error code.
	return 0;
}