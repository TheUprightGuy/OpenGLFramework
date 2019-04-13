#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "utils.h"
#include "CameraManager.h"

class CLight
{
public:
	CLight(GLuint& _program);
	~CLight();

	void Position(glm::vec3 _pos) { m_lightPos = _pos; }
	void Render();

private:
	GLuint m_program;
	glm::vec3 m_lightColor;
	glm::vec3 m_lightPos;
	glm::vec3 m_ambientColor;

	float m_lightSpecStr;
	float m_shininess;
	float m_ambientStrength;
};
#endif // !__LIGHT_H__
