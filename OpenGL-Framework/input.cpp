#include "input.h"
#include "CameraManager.h"

CInput* CInput::s_pInput = nullptr;

void CInput::UpdateMousePicking()
{
	//m_MousePos
	/*
	(GLfloat)glutGet(GLUT_WINDOW_WIDTH);
	 (GLfloat)glutGet(GLUT_WINDOW_HEIGHT);
	*/

	float mouseX = (2.0f * m_MousePos.x) / (float)glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
	float mouseY = 1.0f - (2.0f * m_MousePos.y) / (float)glutGet(GLUT_WINDOW_HEIGHT);

	glm::vec2 normalizedScreenPos = glm::vec2(mouseX, mouseY);

	//screen pos to ProjSpace
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);

	//ProjSpace to eye space
	glm::mat4 invProjMat = glm::inverse(CCameraManager::GetInstance().GetCam()->GetProj());

	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//eyespaceto world space
	glm::mat4 invViewMat = glm::inverse(CCameraManager::GetInstance().GetCam()->GetView());
	glm::vec4 rayWorld = invViewMat * eyeCoords;

	m_LookVec = glm::normalize(glm::vec3(rayWorld));
}

CInput::CInput()
{

	m_CurrentState = INPUT_RELEASE;
	m_LookVec = glm::vec3(0.0f, 0.0f, 0.0f);

}

CInput::~CInput()
{
	m_CurrentState = INPUT_RELEASE;
}


