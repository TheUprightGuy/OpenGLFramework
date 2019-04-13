#ifndef __CUBEMESH_H__
#define __CUBEMESH_H__

#include "utils.h"

class CCubeMesh
{
public:
	CCubeMesh();
	~CCubeMesh();
	void Render()
	{
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

private:
	GLuint m_ebo;
	GLuint m_vao;
	GLuint m_vbo;
};

#endif // !__CUBEMESH_H__
