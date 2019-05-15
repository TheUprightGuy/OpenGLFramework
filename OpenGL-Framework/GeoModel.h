#pragma once
#include "utils.h"
#include "CameraManager.h"
#include "camera.h"
class CGeometryModel
{
public:
	CGeometryModel(GLuint program);
	~CGeometryModel();

	void Render();
	
	void SetPosition(glm::vec3 _newPos) { m_position = _newPos; }

private:
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_program;

	CCamera* m_pCamera;

	glm::vec3 m_position;
};