#include "FrameBuffer.h"

CFrameBuffer::CFrameBuffer(GLuint program)
	:m_program(program)
{
	GLfloat fVertices[] = {
		// Positions            // Tex Coords
		-1.0f, 1.0f, 0.0f,        0.0f, 1.0f,            // Top Left
		1.0f, 1.0f, 0.0f,        1.0f, 1.0f,            // Top Right
		1.0f, -1.0f, 0.0f,        1.0f, 0.0f,            // Bottom Right
		-1.0f, -1.0f, 0.0f,        0.0f, 0.0f,            // Bottom Left
	};

	GLuint m_indices[] =
	{
		0, 1, 2, // First Triangle
		0, 2, 3 // Second Triangle
	};
	
	GLfloat fWidth = (GLfloat)glutGet(GLUT_WINDOW_WIDTH);
	GLfloat fHeight = (GLfloat)glutGet(GLUT_WINDOW_HEIGHT);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVertices), fVertices, GL_STATIC_DRAW);

	GLuint m_ebo;
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(m_indices),
		m_indices,
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Offset);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glGenTextures(1, &m_renderTexture); glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fWidth, fHeight,
		0, //border 
		GL_RGB, //format
		GL_UNSIGNED_BYTE, //data type ,
		NULL); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		m_renderTexture,
		0);

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);

	
	glRenderbufferStorage(GL_RENDERBUFFER,
		GL_DEPTH24_STENCIL8, fWidth, fHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		m_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
}

void CFrameBuffer::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(m_program);

	glActiveTexture(GL_TEXTURE0); 
	glUniform1i(glGetUniformLocation(m_program, "renderTexture"), 0); 
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);

	glUniform1f(glGetUniformLocation(m_program, "fTime"), (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000);
	glUniform1f(glGetUniformLocation(m_program, "fWidth"), (GLfloat)glutGet(GLUT_WINDOW_WIDTH));
	glUniform1f(glGetUniformLocation(m_program, "fHeight"), (GLfloat)glutGet(GLUT_WINDOW_HEIGHT));

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
