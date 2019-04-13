#pragma once
#ifndef __STENCIL_OBJ__
#define __STENCIL_OBJ__

#include "utils.h"
#include "object.h"
#include "CameraManager.h"
class CStencilObj
{
public:
	CStencilObj(CObject* _original);
	~CStencilObj()
	{
		delete m_secondary;
	}

	void Render();
	void process();
	void setcolor(glm::vec3 _color) { m_color = _color; }
	void setsize(glm::vec3 _size) { m_scale += _size; }
private:
	glm::vec3 m_color;
	glm::vec3 m_scale;

	CObject* m_original;
	CObject* m_secondary;
};


#endif