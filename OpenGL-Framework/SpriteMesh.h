#ifndef __SPRITEMESH_H__
#define __SPRITEMESH_H__

#include "utils.h"


class CSpriteMesh
{
public:
	CSpriteMesh();
	~CSpriteMesh();
	void Render()
	{
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	GLuint m_ebo;
	GLuint m_vao;
	GLuint m_vbo;
};

#endif
