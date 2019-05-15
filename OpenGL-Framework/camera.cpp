#include "camera.h"
#include <math.h>

CCamera::CCamera(bool bIsOrtho)
{
	m_CamPos = glm::vec3(0.0f, 50.0f, 0.0f);
	m_TargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_UpVec = glm::vec3(0.0f, 1.0f, 0.0f);

	//m_View = glm::lookAt(
	//	m_CamPos,
	//	m_TargetPos - m_CamPos,
	//	m_UpVec);
	//Weird shit going on

	glm::vec3 camPos = GetCamPos();
	camPos *= -1;

	glm::mat4 view; /*= glm::translate(glm::mat4(), camPos);*/

	view *= glm::rotate(glm::mat4(), glm::radians(0.0f), { 1.0f, 0.0f, 0.0f });
	view *= glm::rotate(glm::mat4(), glm::radians(0.0f), { 0.0f, 1.0f, 0.0f });
	view *= glm::rotate(glm::mat4(), glm::radians(0.0f), { 0.0f, 0.0f, 1.0f });
	view *= glm::translate(glm::mat4(), camPos);

	m_View = view; //I'll do it myself then >:(

	GLfloat fWidth = (GLfloat)glutGet(GLUT_WINDOW_WIDTH);
	GLfloat fHeight = (GLfloat)glutGet(GLUT_WINDOW_HEIGHT);

	if (bIsOrtho)
	{
		m_Proj = glm::ortho(0.0f, 100.0f, 50.0f, 0.0f);
	}
	else
	{
		float toRadians = glm::radians(80.0f);
		m_Proj = glm::perspectiveFov(toRadians, fWidth, fHeight, 0.1f, 10000.0f);
	}
}

CCamera::~CCamera()
{
}



