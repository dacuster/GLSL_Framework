#pragma once

/// <summary> Shader code to run on the program. </summary>
class Shader
{
public:
	Shader();
	~Shader();

	/// <summary> Initialize the shader program. </summary>
	void initialize();

	/// <summary> Load a shader from a file. </summary>
	void load(GLenum _type, const std::string& _filename);

	/// <summary> Link the shader to the program. </summary>
	void link();

	/// <summary> Validate the shader to the program. </summary>
	void validate();

	/// <summary> Get the uniform variable location for the projection matrix. </summary>
	GLuint getProjectionLocation();

	/// <summary> Get the uniform variable location for the model matrix. </summary>
	GLuint getModelLocation();

	/// <summary> Get the uniform variable location for the view matrix. </summary>
	GLuint getViewLocation();

	/// <summary> Use the shader in the program. </summary>
	void use();

	/// <summary> Clear the shader from the program. </summary>
	void clear();

	/// <summary> Load all the uniforms into the shader. </summary>
	void loadUniforms();

	/// <summary> Load a uniform veriable location from the shader. </summary>
	GLuint loadUniform(const GLchar* _pVariable);

private:
	/// <summary> Id of the shader program. </summary>
	GLuint mId;

	/// <summary> Uniform projection matrix for the shader. </summary>
	GLuint mUniformProjection;

	/// <summary> Uniform model matrix for the shader. </summary>
	GLuint mUniformModel;

	/// <summary> Uniform view matrix for the shader. </summary>
	GLuint mUniformView;

	/// <summary> Compile the shader for the program. </summary>
	void compile(GLenum _type, const char* _pContent);
};