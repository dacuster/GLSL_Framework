#pragma once

class Mesh
{
public:
	Mesh();
	~Mesh();

	/// <summary> Create the mesh. </summary>
	void create(GLfloat* _pVertices, unsigned int* _pIndices, unsigned int _vertexCount, unsigned int _indexCount);

	/// <summary> Render the mesh. </summary>
	void render();

	/// <summary> Clear the mesh. </summary>
	void clear();

private:
	/// <summary> Vertex array object. </summary>
	GLuint mVAO;

	/// <summary> Vertex buffer object. </summary>
	GLuint mVBO;

	/// <summary> Index buffer object. </summary>
	GLuint mIBO;

	/// <summary> Number of indices. </summary>
	GLsizei mIndexCount;
};