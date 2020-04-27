#include <string>
#include <fstream>

#include <GL/glew.h>

#include <Shader.h>

Shader::Shader()
{
	// Set all the member variables to null.
	mId = 0;
	mUniformModel = 0;
	mUniformProjection = 0;
	mUniformView = 0;
}

Shader::~Shader()
{
}

void Shader::initialize()
{
	// Create a program and set the id.
	mId = glCreateProgram();

	// Couldn't create the shader program.
	if (!mId)
	{
		printf("Error creating shader program!\n");
	}
}

void Shader::load(GLenum _type, const std::string& _filename)
{
	// Pass the file into fstream for loading.
	std::ifstream fileStream(_filename, std::ios::in);

	// Placeholder for file content.
	std::string content;

	// Current line in the file.
	std::string line = "";

	// Loop through each line in the file.
	while (!fileStream.eof())
	{
		// Get the current line in the stream.
		std::getline(fileStream, line);

		// Append the line to the end of the content string.
		content.append(line + "\n");
	}

	// Close the stream.
	fileStream.close();

	// Compile the shader.
	compile(_type, (content.c_str()));
}

void Shader::link()
{
	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	// Link the program.
	glLinkProgram(mId);

	// Check the link status.
	glGetProgramiv(mId, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(mId, sizeof(errorLog), NULL, errorLog);
		printf("Error linking program: '%s'\n", errorLog);
	}
}

void Shader::validate()
{
	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	// Validate the program.
	glValidateProgram(mId);

	// Check the validate status.
	glGetProgramiv(mId, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(mId, sizeof(errorLog), NULL, errorLog);
		printf("Error validating program: '%s'\n", errorLog);
	}
}

GLuint Shader::getProjectionLocation()
{
	return mUniformProjection;
}

GLuint Shader::getModelLocation()
{
	return mUniformModel;
}

GLuint Shader::getViewLocation()
{
	return mUniformView;
}

void Shader::use()
{
	glUseProgram(mId);
}

void Shader::clear()
{
	glUseProgram(0);
}

void Shader::loadUniforms()
{
	mUniformModel = glGetUniformLocation(mId, "uModel");
	mUniformProjection = glGetUniformLocation(mId, "uProjection");
	mUniformView = glGetUniformLocation(mId, "uView");
}

GLuint Shader::loadUniform(const GLchar* _pVariable)
{
	return glGetUniformLocation(mId, _pVariable);
}

void Shader::compile(GLenum _type, const char* _pContent)
{
	// Create a shader based on type.
	GLuint shader = glCreateShader(_type);

	// Convert the shader contnent to a c string.
	const GLchar* content[1];
	content[0] = _pContent;

	// Get the length of the content.
	GLint contentLength[1];
	contentLength[0] = (GLint)strlen(content[0]);

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
	glAttachShader(mId, shader);
}
