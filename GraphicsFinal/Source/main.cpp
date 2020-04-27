// Windows libraries.
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>

// GL libraries.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// Project libraries.
#include <Mesh.h>
#include <Shader.h>
#include <GL_Window.h>

#define PI 3.14159265

// Window dimentions.
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

// Convert degrees to radians.
const float toRadians = (float)PI / 180.0f;

// Field of view (y-direction).
const float fieldOfView = 45.0f;

// Meshes.
std::vector<Mesh*> meshes;

// Shaders.
std::vector<Shader*> shaders;

// Window.
GL_Window mainWindow;

// Shader file locations.
static const char* vertexShaderFile = "resources/vs/shader.vert";
static const char* fragmentShaderFile = "resources/fs/shader.frag";

void CreateObject()
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

	Mesh* pMesh1 = new Mesh();

	pMesh1->create(verticies, indices, 12, 12);

	meshes.push_back(pMesh1);
}

void CreateShaders()
{
	// Create a new shader.
	Shader* pShader = new Shader();

	// Initialize the shader.
	pShader->initialize();

	// Load the vertex and fragment shaders.
	pShader->load(GL_VERTEX_SHADER, vertexShaderFile);
	pShader->load(GL_FRAGMENT_SHADER, fragmentShaderFile);

	// Link the shaders.
	pShader->link();

	// Validate the shaders.
	pShader->validate();

	// Load the uniforms.
	pShader->loadUniforms();

	// Add the shader to the list.
	shaders.push_back(pShader);
}

int main()
{
	// Create a window.
	mainWindow = GL_Window(WIDTH, HEIGHT);

	// Initialize the window.
	mainWindow.initialize();

	// Create an object.
	CreateObject();

	// Create the shaders.
	CreateShaders();

	// Get the aspect ratio of the screen.
	GLfloat aspectRatio = (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight();

	// Model and projection matrices.
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	// Create projection matrix.
	glm::mat4 projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 100.0f);

	// Loop until window is closed.
	while (!mainWindow.shouldClose())
	{
		// Get and handle user input event.
		glfwPollEvents();

		// Clear the window to black.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Clear the color buffer data.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the shader program.
		shaders[0]->use();

		// Get the uniforms.
		uniformModel = shaders[0]->getModelLocation();
		uniformProjection = shaders[0]->getProjectionLocation();

		// Identity model matrix.
		glm::mat4 model(1.0f);

		// Apply matrix operations.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
			
		// Apply the value to the uniform variable at their location.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Render the meshes.
		meshes[0]->render();

		// Swap to back buffer.
		mainWindow.swapBuffers();
	}

	// Return error code.
	return 0;
}