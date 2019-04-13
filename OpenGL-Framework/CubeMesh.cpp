#include "CubeMesh.h"

GLfloat m_cubevertices[] =
{
	// Positions // Colors // TexCoords
	// Positions // Colors // TexCoords
	// Front Face
	-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// Right Face
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// Back Face
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	// Left Face
	-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// Top Face
	-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	// Bottom Face
	-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,


};

GLuint m_cubeindices[] =
{
	// Top Face
	16, 17, 18,
	16, 18, 19,

	// Bottom Face
	20, 21, 22,
	20, 22, 23,

	// Left Face
	12, 13, 14,
	12, 14, 15,

	// Right Face
	4, 5, 6,
	4, 6, 7,

	// Front Face
	0, 1, 2,
	0, 2, 3,

	// Back Face
	8, 9, 10,
	8, 10, 11,
};


CCubeMesh::CCubeMesh()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubevertices), m_cubevertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(m_cubeindices),
		m_cubeindices,
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

CCubeMesh::~CCubeMesh()
{

}