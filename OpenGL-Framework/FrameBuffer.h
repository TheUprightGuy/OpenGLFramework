#pragma once
#include "utils.h"
#include "camera.h"
#include "CameraManager.h"

class CFrameBuffer
{
public:
	CFrameBuffer(GLuint program);
	~CFrameBuffer();

	void Render();

	GLuint GetFBO() { return(m_framebuffer); }
private:
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_rbo;

	GLuint m_program;

	GLuint m_renderTexture;
	GLuint m_framebuffer;
};

