#include <string>

#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include <ShaderUtility.h>

void ShaderUtility::compileShader(GLenum _type, const std::string& _content)
{
	// Create a shader based on type.
	GLuint shader = glCreateShader(_type);

	// Convert the shader contnent to a c string.
	const GLchar* content[1];
	content[0] = _content.c_str();

	// Get the length of the content.
	GLint contentLength[1];
	contentLength[0] = strlen(content[0]);

	// Pass the contnent into the shader source.
	glShaderSource(shader, 1, content, contentLength);

	// Compile the shader.
	glCompileShader(shader);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	// Check the compile status.
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
		printf("Error compiling the %d shader: '%s'\n", _type, errorLog);
		return;
	}

	// Attach shader to the program.
	glAttachShader(mProgramId, shader);
	
	return;
}

void ShaderUtility::initialize()
{
	// Create a program and set the id.
	mProgramId = glCreateProgram();

	// Couldn't create the shader program.
	if (!mProgramId)
	{
		printf("Error creating shader program!\n");
	}

	return;
}

void ShaderUtility::loadShader(GLenum _type, const std::string& _file)
{
	// Pass the file into fstream for loading.
	std::ifstream file;

	file.open(_file, std::ios::in);

	std::string content;

	std::string line = "";

	if (file.good())
	{
		while (!file.eof())
		{
			std::getline(file, line);
			content.append(line + "\n");
		}
	}

	file.close();

	// Compile the shader.
	compileShader(_type, content);

	return;
}

void ShaderUtility::linkProgram()
{
	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	// Link the program.
	glLinkProgram(mProgramId);

	// Check the link status.
	glGetProgramiv(mProgramId, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(mProgramId, sizeof(errorLog), NULL, errorLog);
		printf("Error linking program: '%s'\n", errorLog);
	}

	return;
}

void ShaderUtility::validateProgram()
{
	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	// Validate the program.
	glValidateProgram(mProgramId);

	// Check the validate status.
	glGetProgramiv(mProgramId, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(mProgramId, sizeof(errorLog), NULL, errorLog);
		printf("Error validating program: '%s'\n", errorLog);
	}

	return;
}

void ShaderUtility::useProgram()
{
	glUseProgram(mProgramId);

	return;
}

void ShaderUtility::stopProgram()
{
	glUseProgram(0);

	return;
}

void ShaderUtility::deleteProgram()
{
	glDeleteProgram(mProgramId);

	return;
}

GLuint ShaderUtility::loadUniform(const GLchar* _variable)
{
	return glGetUniformLocation(mProgramId, _variable);
}
