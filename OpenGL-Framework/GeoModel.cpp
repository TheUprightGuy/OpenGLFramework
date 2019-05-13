#include "GeoModel.h"

CGeometryModel::CGeometryModel(GLuint program) 
{
	m_program = program;
	m_pCamera = CCameraManager::GetInstance().GetCam();

	m_position = { 0.0f, 0.0f , 0.0f };

	GLfloat points[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);

	glGenVertexArrays(1, &m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);

}

void CGeometryModel::Render()
{
	glUseProgram(m_program);

	glm::mat4 model;
	model = glm::translate(model, m_position);

	glm::mat4 mvp = m_pCamera->GetProj() * m_pCamera->GetView() * model;

	GLint vpLoc = glGetUniformLocation(m_program, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
