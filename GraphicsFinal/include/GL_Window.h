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
	bool shouldClose() { return glfwWindowShouldClose(mpWindow); }

	/// <summary Get the current key status. </summary>
	bool* getKeys() { return mKeys; }

	/// <summary Get the current mouse button status. </summary>
	bool* getButtons() { return mMouseButtons; }

	/// <summary Get the change in the mouse x coordinate. </summary>
	GLfloat getMouseDeltaX();

	/// <summary Get the change in the mouse y coordinate. </summary>
	GLfloat getMouseDeltaY();

	/// <summary> Swap the back and front buffers. </summary>
	void swapBuffers() { glfwSwapBuffers(mpWindow); }

private:
	/// <sumary> The window of the application. </summary>
	GLFWwindow* mpWindow;

	/// <summary> Width of the window. </summary
	GLint mWidth;

	/// <summary> Height of the window. </summary
	GLint mHeight;

	/// <summary> Buffer width of the window. </summary
	GLint mBufferWidth;

	/// <summary> Buffer height of the window. </summary
	GLint mBufferHeight;

	/// <summary> Array of keys in ASCII format. </summary>
	bool mKeys[1024];

	/// <summary> Array of mouse buttons. </summary>
	bool mMouseButtons[8];

	/// <summary> Previous mouse x coordinate. </summary>
	GLfloat mLastMouseX = 0.0f;
	/// <summary> Previous mouse y coordinate. </summary>
	GLfloat mLastMouseY = 0.0f;
	/// <summary> Changed mouse x coordinate. </summary>
	GLfloat mDeltaMouseX = 0.0f;
	/// <summary> Changed mouse y coordinate. </summary>
	GLfloat mDeltaMouseY = 0.0f;

	/// <summary> Previous mouse wheel x coordinate. </summary>
	GLfloat mLastMouseWheelX = 0.0f;
	/// <summary> Previous mouse wheel y coordinate. </summary>
	GLfloat mLastMouseWheelY = 0.0f;
	/// <summary> Changed mouse wheel x coordinate. </summary>
	GLfloat mDeltaMouseWheelX = 0.0f;
	/// <summary> Changed mouse wheel y coordinate. </summary>
	GLfloat mDeltaMouseWheelY = 0.0f;

	/// <summary> Check for the first movement of the mouse. </summary>
	bool mMouseFirstMoved = true;

	bool mMouseFirstClick = false;

	/// <summary> Check for the first scroll of the mouse. </summary>
	bool mMouseFirstScroll = true;

	// <summary> Create callback for events. </summary>
	void createCallbacks();

	/// <summary> Handle key event callbacks. </summary>
	static void handleKeys(GLFWwindow* _pWindow, int _key, int _code, int _action, int _mode);

	/// <summary> Handle mouse event movement callbacks. </summary>
	static void handleMouse(GLFWwindow* _pWindow, double _xPosition, double _yPosition);

	/// <summary> Handle mouse event button callbacks. </summary>
	static void handleMouseButton(GLFWwindow* _pWindow, int _button, int _action, int _mods);

	/// <summary> Handle window event callbacks. </summary>
	static void handleWindow(GLFWwindow* _pWindow, int _width, int _height);

	/// <summary> Initialize all the keys to false. </summary>
	void initializeKeys();

	/// <summary> Initialize all the mouse buttons to false. </summary>
	void initializeMouseButtons();
};