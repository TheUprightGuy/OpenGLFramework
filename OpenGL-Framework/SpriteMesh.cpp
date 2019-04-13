#include "SpriteMesh.h"

GLfloat m_vertices[] =
{
	// Positions // Colors
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Top Left
	 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Top Right
	 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Bottom Right
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Bottom Left
};

GLuint m_indices[] =
{
	0, 1, 2, // First Triangle
	0, 2, 3 // Second Triangle
};


CSpriteMesh::CSpriteMesh()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(m_indices),
		m_indices,
		GL_STATIC_DRAW);


	//Shape
	glVertexAttribPointer(
		0, // Layout location on vertex shader
		3, // 3 float components (eg position)
		GL_FLOAT, // Type of data
		GL_FALSE, // Data normalized?
		8 * sizeof(GLfloat), // Stride of the entire single vertex
		(GLvoid*)0); // Offset
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(
		1, // Layout location on vertex shader
		3, // 3 float components (eg position)
		GL_FLOAT, // Type of data
		GL_FALSE, // Data normalized?
		8 * sizeof(GLfloat), // Stride of the entire single vertex
		(GLvoid*)(3 * sizeof(GLfloat))); // Offset
	glEnableVertexAttribArray(1);

	//Texture
	/*************************************************/
	glVertexAttribPointer(
		2, // Layout location on vertex shader
		2, // 2 float components for texture coords
		GL_FLOAT, // Type of data
		GL_FALSE, // Data normalized?
		8 * sizeof(GLfloat), // Stride
		(GLvoid*)(6 * sizeof(GLfloat))); // Offset
	glEnableVertexAttribArray(2);

}

CSpriteMesh::~CSpriteMesh()
{

}

