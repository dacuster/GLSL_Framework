#pragma once

// Utility class to load, compile and attach a vertex and fragment shader to the program.
class ShaderUtility
{
private:
	GLuint mProgramId;

	void compileShader(GLenum _type, const std::string& _file);

public:
	ShaderUtility() {}
	~ShaderUtility() {}

	// Initialize the shader utility.
	void initialize();

	// Load a shader file.
	void loadShader(GLenum _type, const std::string& _file);

	// Link the program.
	void linkProgram();

	// Validate the program.
	void validateProgram();

	// Use the program.
	void useProgram();

	// Stop using the program.
	void stopProgram();

	// Delete the program.
	void deleteProgram();
};