#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "utils.h"

#include <string>

class CCamera
{
public:
	CCamera(bool bIsOrtho);
	~CCamera();

	glm::vec3 GetCamPos() { return(m_CamPos); }
	glm::mat4 GetView();
	glm::mat4 GetProj() { return(m_Proj); }

	void CamTranslate(glm::vec3 _newPos) { m_CamPos = _newPos; }
	void CamTarget(glm::vec3 _newPos) { m_TargetPos = _newPos; }

private:
	glm::vec3 m_CamPos;
	glm::vec3 m_TargetPos;

	glm::mat4 m_View;
	glm::mat4 m_Proj;

};
#endif // !__CAMERA_H__
