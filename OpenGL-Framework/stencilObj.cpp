#include "stencilObj.h"

CStencilObj::CStencilObj(CObject * _original)
	:m_original(_original)
{
	initinfo stencilinfo;
	stencilinfo.objPosition = m_original->GetPos();
	stencilinfo.RotationDegrees = m_original->GetRot();
	stencilinfo.RotAxis = m_original->GetRotAxis();
	
	m_scale = glm::vec3(0.1f, 0.1f, 0.1f);
	m_color = glm::vec3(1.0f, 0.0f, 0.0f);

	stencilinfo.color = m_color;
	m_scale = m_original->GetScale() + m_scale;
	stencilinfo.objScale = m_scale;

	m_secondary = new CObject(DEFAULT, m_original->GetMeshID(), stencilinfo);

}

void CStencilObj::Render()
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //stPass, dpFail, bothPass

	glStencilFunc(GL_ALWAYS, // test function
		1,// current value to set
		0xFF);//mask value,
	glStencilMask(0xFF);//enable writing to stencil buffer

	m_original->Render(CCameraManager::GetInstance().GetCam());
	
	glStencilMask(0x00); //disable writing to stencil buffer
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);


	m_secondary->Render(CCameraManager::GetInstance().GetCam());
	glStencilMask(0x00);
	glDisable(GL_STENCIL_TEST);
}

void CStencilObj::process()
{
	m_secondary->Translate(m_original->GetPos());
	m_secondary->Scale(m_scale);
	//m_secondary->Rotation(m_original->GetRot(), m_original->GetRotAxis());
}
