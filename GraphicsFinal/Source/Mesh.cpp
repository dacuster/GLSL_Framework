
#include <GL/glew.h>

#include <Mesh.h>

Mesh::Mesh()
{
	// Set everything to null.
	mVAO = 0;
	mVBO = 0;
	mIBO = 0;
	mIndexCount = 0;
}

Mesh::~Mesh()
{
	// Clear mesh from graphics memory.
	clear();
}

void Mesh::create(GLfloat* _pVertices, unsigned int* _pIndices, unsigned int _vertexCount, unsigned int _indexCount)
{
	// Set the number of indices.
	mIndexCount = _indexCount;
	
	// Create a vertex array object.
	glGenVertexArrays(1, &mVAO);

	// Bind the VAO.
	glBindVertexArray(mVAO);


	// Create an index buffer object.
	glGenBuffers(1, &mIBO);

	// Bind the IBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

	// Pass indices into the buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_pIndices[0]) * mIndexCount, _pIndices, GL_STATIC_DRAW);


	// Create a vertex buffer object.
	glGenBuffers(1, &mVBO);

	// Bind the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	// Pass verticies into buffer. Not going to be edited (GL_STATIC_DRAW)
	glBufferData(GL_ARRAY_BUFFER, sizeof(_pVertices[0]) * _vertexCount, _pVertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the IBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Unbind the VAO
	glBindVertexArray(0);
}

void Mesh::render()
{
	// Use this VAO for the shader.
	glBindVertexArray(mVAO);

	// Bind the IBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

	// Draw the vertices.
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);

	// Unbind the IBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Unbind the VAO.
	glBindVertexArray(0);
}

void Mesh::clear()
{
	// Check for existing IBO.
	if (mIBO != 0)
	{
		// Delete the IBO from graphics memory.
		glDeleteBuffers(1, &mIBO);

		// Clear the IBO.
		mIBO = 0;
	}

	// Check for existing VBO.
	if (mVBO != 0)
	{
		// Delete the VBO from graphics memory.
		glDeleteBuffers(1, &mVBO);

		// Clear the VBO.
		mVBO = 0;
	}

	// Check for existing VAO.
	if (mVAO != 0)
	{
		// Delete the VAO from graphics memory.
		glDeleteVertexArrays(1, &mVAO);

		// Clear the VAO.
		mVAO = 0;
	}

	// Reset the index count.
	mIndexCount = 0;
}
