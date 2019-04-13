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

