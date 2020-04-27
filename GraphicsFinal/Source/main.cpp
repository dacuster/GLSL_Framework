// Windows libraries.
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

// GL libraries.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// Project libraries.
#include <ShaderUtility.h>

#define PI 3.14159265

// Window dimentions.
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

// Convert degrees to radians.
const float toRadians = PI / 180.0f;

// Field of view (y-direction).
const float fieldOfView = 45.0f;

/// <summary> Vertex array object. </summary>
GLuint VAO;

/// <summary> Vertex buffer object. </summary>
GLuint VBO;

/// <summary> Index buffer object </summary>
GLuint IBO;

// Uniform data.
GLuint uniformModel;
GLuint uniformProjection;

// Translation parameters.
bool direction = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.01f;

// Rotation parameters.
float currentAngle = 0.0f;
float angleIncrement = 1.0f;

// Scaling parameters.
bool sizeDirection = true;
float currentSize = 0.4f;
float maximumSize = 0.8f;
float minimumSize = 0.1f;

const std::string vertexShaderFile = "resources\\vs\\shader.vert";
const std::string fragmentShaderFile = "resources\\fs\\shader.frag";

void CreateTriangle()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat verticies[] = {
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	// Create a vertex array object.
	glGenVertexArrays(1, &VAO);

	// Bind the VAO.
	glBindVertexArray(VAO);


	// Create an index buffer object.
	glGenBuffers(1, &IBO);

	// Bind the IBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Pass indices into the buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Create a vertex buffer object.
	glGenBuffers(1, &VBO);
		
	// Bind the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Pass verticies into buffer. Not going to be edited (GL_STATIC_DRAW)
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind the VAO
	glBindVertexArray(0);
		
	// Unbind the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the IBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

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

	// Get the location of the uniform variable.
	uniformModel = shaderUtility.loadUniform("model");
	uniformProjection = shaderUtility.loadUniform("projection");

	// Get the aspect ratio of the screen.
	GLfloat aspectRatio = (GLfloat)bufferWidth / (GLfloat)bufferHeight;

	// Create projection matrix.
	glm::mat4 projection = glm::perspective(fieldOfView, aspectRatio, 0.1f, 100.0f);

	// Loop until window is closed.
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user input event.
		glfwPollEvents();

		if (direction)
		{
			triangleOffset += triangleIncrement;
		}
		else
		{
			triangleOffset -= triangleIncrement;
		}

		if (abs(triangleOffset) >= triangleMaxOffset)
		{
			direction = !direction;
		}

		currentAngle += angleIncrement;

		if (currentAngle >= 360.0)
		{
			currentAngle -= 360.0f;
		}

		if (sizeDirection)
		{
			currentSize += 0.01f;
		}
		else
		{
			currentSize -= 0.01f;
		}

		if (currentSize >= maximumSize || currentSize <= minimumSize)
		{
			sizeDirection = !sizeDirection;
		}

		// Clear the window to black.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Clear the color buffer data.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the shader program.
		shaderUtility.useProgram();

		// Identity model matrix.
		glm::mat4 model(1.0f);

		// Apply matrix operations.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
			
		// Apply the value to the uniform variable at their location.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Use this VAO for the shader.
		glBindVertexArray(VAO);
		
		// Bind the IBO.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		// Draw the vertices.
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		// Unbind the IBO.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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