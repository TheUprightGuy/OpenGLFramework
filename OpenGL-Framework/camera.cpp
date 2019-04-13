#include "camera.h"

CCamera::CCamera(bool bIsOrtho)
{
	m_CamPos = glm::vec3(0.0f, 0.0f, 5.0f);
	m_TargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.8f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	GLfloat fWidth = (GLfloat)glutGet(GLUT_WINDOW_WIDTH);
	GLfloat fHeight = (GLfloat)glutGet(GLUT_WINDOW_HEIGHT);

	if (bIsOrtho)
	{
		m_Proj = glm::ortho(0.0f, 100.0f, 50.0f, 0.0f);
	}
	else
	{
		m_Proj = glm::perspectiveFov(45.0f, fWidth, fHeight, 0.1f, 10000.0f);
	}
}

CCamera::~CCamera()
{
}

glm::mat4 CCamera::GetView()
{
	m_View = 
		glm::lookAt(
		m_CamPos,
		m_TargetPos - m_CamPos,
		glm::vec3(0.0f, 1.0f, 0.0f));

	return(m_View);
}
