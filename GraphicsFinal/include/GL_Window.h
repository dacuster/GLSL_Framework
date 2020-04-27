#pragma once

class GL_Window
{
public:
	GL_Window();

	/// <summary> Create a window with the given width and height. </summary>
	GL_Window(GLint _width, GLint _height);

	~GL_Window();

	/// <summary> Initialize the window. </summary>
	int initialize();

	/// <summary> Get the buffer width of the window. </summary>
	GLint getBufferWidth() { return mBufferWidth; }
	/// <summary> Get the buffer height of the window. </summary>
	GLint getBufferHeight() { return mBufferHeight; }

	/// <summary> Should the window close? </summary>
	bool shouldClose() { return glfwWindowShouldClose(pWindow); }

	/// <summary> Swap the back and front buffers. </summary>
	void swapBuffers() { glfwSwapBuffers(pWindow); }

private:
	/// <sumary> The window of the application. </summary>
	GLFWwindow* pWindow;

	/// <summary> Width of the window. </summary
	GLint mWidth;

	/// <summary> Height of the window. </summary
	GLint mHeight;

	/// <summary> Buffer width of the window. </summary
	GLint mBufferWidth;

	/// <summary> Buffer height of the window. </summary
	GLint mBufferHeight;
};