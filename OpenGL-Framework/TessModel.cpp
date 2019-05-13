#include "TessModel.h"

CTessModel::CTessModel(GLuint program)
{
	m_program = program;
	m_pCamera = CCameraManager::GetInstance().GetCam();

	GLfloat points[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0, 1.0, 0.0f
	};
	glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch

	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindVertexArray(0);
}

void CTessModel::Render()
{
	glUseProgram(m_program);

	glPolygonMode(GL_FRONT, GL_LINE);

	glm::mat4 model;
	model = glm::translate(model, m_position);
	glm::mat4 mvp = m_pCamera->GetProj() * m_pCamera->GetView() * model;

	GLint mvLoc = glGetUniformLocation(m_program, "mvp");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT, GL_FILL);
}
