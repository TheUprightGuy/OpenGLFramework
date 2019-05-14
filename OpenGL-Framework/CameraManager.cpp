#include "CameraManager.h"


CCameraManager* CCameraManager::s_pCameraManager = nullptr;

CCameraManager::CCameraManager(bool _createStarterCam /* = true */)
{
	m_iCurrentCam = 0;

	if (_createStarterCam)
	{
		m_pCameras.push_back(new CCamera(false));
	}
}

CCameraManager::~CCameraManager(){}
void CCameraManager::init(){}

int CCameraManager::AddNewCam()
{
	m_pCameras.push_back(new CCamera(false));
	return(m_pCameras.size() - 1);
}

CCamera * CCameraManager::GetCam(int _iIndex)
{
	if (m_pCameras.empty() || (m_iCurrentCam > m_pCameras.size() ) )
	{
		return (nullptr);
	}
	else
	{
		return(m_pCameras[m_iCurrentCam]);
	}
}

CCamera * CCameraManager::GetCam()
{
	if (m_pCameras.empty() || m_iCurrentCam > m_pCameras.size())
	{
		return (nullptr);
	}
	else
	{
		return(m_pCameras[m_iCurrentCam]);
	}
}

CCamera * CCameraManager::GetOrthoCam()
{
	if (orthoCam == nullptr)
	{
		orthoCam = new CCamera(true);
	}

	return(orthoCam);
}

void CCameraManager::DebugCamera(float _deltaTime, float _movementSpeed)
{
	glutSetCursor(GLUT_CURSOR_NONE);

	float distance = _movementSpeed * _deltaTime;

	static float yaw = 0.0f;
	static float pitch = 0.0f;
	static float roll = 0.0f;

	float dx = std::sin(glm::radians(yaw))/** distance*/;
	float dz = std::cos(glm::radians(yaw))/** distance*/;

	glm::vec3 movement = {0.0f, 0.0f, 0.0f};

	CInput::GetInstance().UpdateMousePicking();
	glm::vec3 movementDir = CInput::GetInstance().GetLookDirection();

	glm::vec3 leftVec = movementDir * distance;
	leftVec.y = 0.0f;
	leftVec.x = movementDir.z;
	leftVec.z = -movementDir.x;

	//testing
	if ((CInput::GetInstance().GetKeyState('w') == INPUT_HOLD) || CInput::GetInstance().GetKeyState('W') == INPUT_HOLD)
	{
		movement += movementDir * distance;
	}

	if ((CInput::GetInstance().GetKeyState('a') == INPUT_HOLD) || CInput::GetInstance().GetKeyState('A') == INPUT_HOLD)
	{
		
		movement += leftVec;
		//movement.z -= dz;
	}

	if ((CInput::GetInstance().GetKeyState('s') == INPUT_HOLD) || CInput::GetInstance().GetKeyState('S') == INPUT_HOLD)
	{
		movement -= movementDir * distance;
	}

	if ((CInput::GetInstance().GetKeyState('d') == INPUT_HOLD) || CInput::GetInstance().GetKeyState('D') == INPUT_HOLD)
	{
		movement -= leftVec;
		//movement.z += dx;
	}

	if ((CInput::GetInstance().GetKeyState('r') == INPUT_HOLD) || CInput::GetInstance().GetKeyState('R') == INPUT_HOLD)
	{
		movement.y += distance;
	}
	if ((CInput::GetInstance().GetKeyState('f') == INPUT_HOLD) || CInput::GetInstance().GetKeyState('F') == INPUT_HOLD)
	{
		movement.y -= distance;
	}

	GetCam()->SetCamPos(GetCam()->GetCamPos() + movement);

	glm::vec2 mousePos = { CInput::GetInstance().GetMousePos().x, CInput::GetInstance().GetMousePos().y };
	GLfloat fWidth = (GLfloat)glutGet(GLUT_WINDOW_WIDTH);
	GLfloat fHeight = (GLfloat)glutGet(GLUT_WINDOW_HEIGHT);

	bool center = false;
	if (mousePos.x > fWidth / 2 + 2)
	{
		yaw += 4;
		center = true;
	}
	else if (mousePos.x < fWidth / 2 - 2)
	{
		yaw -= 4;
		center = true;
	}

	if ((mousePos.y > fHeight / 2 + 2) && pitch < 90.0f)
	{
		pitch += 4;
		center = true;
	}
	else if ((mousePos.y < fHeight / 2 - 2) && pitch > -90.0f)
	{
		pitch -= 4;
		center = true;
	}

	if (center)
	{
	
		glutWarpPointer(fWidth / 2, fHeight / 2);
	}

	glm::vec3 camPos = GetCam()->GetCamPos();
	camPos *= -1;

	glm::mat4 view; /*= glm::translate(glm::mat4(), camPos);*/


	view *= glm::rotate(glm::mat4(), glm::radians(pitch), {1.0f, 0.0f, 0.0f});
	view *= glm::rotate(glm::mat4(), glm::radians(yaw), { 0.0f, 1.0f, 0.0f });
	view *= glm::rotate(glm::mat4(), glm::radians(roll), { 0.0f, 0.0f, 1.0f });
	view *= glm::translate(glm::mat4(), camPos);
	GetCam()->SetView(view);

}

CCameraManager & CCameraManager::GetInstance()
{
	if (s_pCameraManager == nullptr)
	{
		s_pCameraManager = new CCameraManager();
	}

	return(*s_pCameraManager);
}

void CCameraManager::DestroyInstance()
{
	delete s_pCameraManager;
	s_pCameraManager = nullptr;
}

