#pragma once
#include "utils.h"
#include "camera.h"
#include "CameraManager.h"

class CTessModel
{
public:
	CTessModel(GLuint program);
	~CTessModel();

	void Render();

	void SetPos(glm::vec3 _newPos) { m_position = _newPos; }

private:
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_program;

	CCamera* m_pCamera;

	glm::vec3 m_position;
};

