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
#include <Camera.h>

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

// Camera.
Camera camera;

// Time variables.
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	Mesh* pMesh1 = new Mesh();

	pMesh1->create(vertices, indices, 12, 12);

	meshes.push_back(pMesh1);
}

void CreateQuad()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};

	Mesh* pMesh1 = new Mesh();

	pMesh1->create(vertices, indices, 12, 6);

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
	//CreateObject();
	CreateQuad();

	// Create the shaders.
	CreateShaders();

	// Create a camera.
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.75f, 0.001f);

	// Get the aspect ratio of the screen.
	GLfloat aspectRatio = (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight();

	// Model matrix.
	GLuint uniformModel = 0;

	// Current time.
	GLuint uniformTime = 0;

	// Resulition of the buffer.
	GLuint uniformResolution = 0;

	// 'Mouse' coordinates as yaw and pitch of camera.
	GLuint uniformMouse = 0;

	// Get the resolution of the window.
	glm::vec2 resolution = glm::vec2((GLfloat)mainWindow.getBufferWidth(), (GLfloat)mainWindow.getBufferHeight());

	uniformTime = shaders[0]->loadUniform("uTime");
	uniformResolution = shaders[0]->loadUniform("uResolution");
	uniformMouse = shaders[0]->loadUniform("uMouse");

	// Loop until window is closed.
	while (!mainWindow.shouldClose())
	{
		// Get the current time.
		GLfloat now = (GLfloat)glfwGetTime();

		// Calculate delta time.
		deltaTime = now - lastTime;

		// Set the last time for the next frame.
		lastTime = now;

		// Get and handle user input event.
		glfwPollEvents();

		// Check for key presses.
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getButtons(), mainWindow.getMouseDeltaX(), mainWindow.getMouseDeltaY());

		glm::vec2 mouse = glm::vec2(camera.getYaw(), camera.getPitch());

		// Clear the window to black.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Clear the color and depth buffer data.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the shader program.
		shaders[0]->use();

		// Get the uniforms.
		uniformModel = shaders[0]->getModelLocation();

		// Identity model matrix.
		glm::mat4 model(1.0f);

		// Apply matrix operations.
		model = glm::translate(model, camera.getPosition());
			
		// Apply the value to the uniform variable at their location.
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1d(uniformTime, glfwGetTime());
		glUniform2fv(uniformResolution, 1, glm::value_ptr(resolution));
		glUniform2fv(uniformMouse, 1, glm::value_ptr(mouse));

		// Render the meshes.
		meshes[0]->render();

		// Swap to back buffer.
		mainWindow.swapBuffers();
	}

	// Return error code.
	return 0;
}