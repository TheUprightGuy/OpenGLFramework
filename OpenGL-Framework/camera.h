#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "utils.h"

#include <string>

class CCamera
{
public:
	CCamera(bool bIsOrtho);
	~CCamera();

	void SetCamPos(glm::vec3 _newPos) { m_CamPos = _newPos; }

	glm::vec3 GetCamPos() { return(m_CamPos); }
	glm::mat4 GetView() { return(m_View); };
	glm::mat4 GetProj() { return(m_Proj); }

	void SetView(glm::mat4 _newView) { m_View = _newView; }
	void UpdateView()
	{
		m_View =
			glm::lookAt(
				m_CamPos,
				m_TargetPos,
				m_UpVec);
	}

	void AddYaw(float fDegrees);
	void AddPitch(float fDegrees);

	void CamTarget(glm::vec3 _newPos) { m_TargetPos = _newPos; }

private:
	glm::vec3 m_CamPos;
	glm::vec3 m_TargetPos;

	glm::mat4 m_View;
	glm::mat4 m_Proj;
	glm::vec3 m_UpVec;
};
#endif // !__CAMERA_H__
