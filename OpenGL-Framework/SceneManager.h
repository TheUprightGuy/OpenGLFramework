#pragma once
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include <vector>
#include <map>
#include <string.h>

#include "utils.h"
#include "input.h"
#include "camera.h"
#include "TextLabel.h"
#include "object.h"
#include "AudioManager.h"
#include "ProgramManager.h"
#include "CCubeMap.h"
#include "light.h"
#include "Model.h"
#include "ShaderLoader.h"
#include "AudioManager.h"
#include "stencilObj.h"

struct Plane
{
	glm::vec3 PlaneNorm;
	glm::vec3 PlanePoint;
};

struct Line
{
	glm::vec3 P1;
	glm::vec3 P2;
};

enum PointPlace
{
	ON_PLANE,
	FRONT_OF_PLANE,
	BACK_OF_PLANE
};


class CSceneManager
{
public:
	~CSceneManager();

	static CSceneManager& GetInstance();
	static void DestroyInstance();

	//Initalize any scenes in here
	void Init();

	void Render();
	void Process();

	PointPlace EvalPoint(Plane PlaneToEval, glm::vec3 PointPos);
	bool EvalLine(Plane PlaneToEval, Line LineToEval, glm::vec3& CollisionPoint);
	bool CheckMouseCollision(CObject* _object);

	glm::vec3 lerpfunc(glm::vec3 vecA, glm::vec3 vecB, float percent)
	{
		return(vecA + percent * (vecB - vecA));
	}

	
protected:
	static CSceneManager* s_pSceneManager;

private:
	CSceneManager();
	CSceneManager(const CSceneManager& _kr) = delete;
	CSceneManager& operator= (const CSceneManager& _kr) = delete;

	//Variables
	GLuint m_program;

	//Global input
	CInput* m_pInput;

	CStencilObj* m_Stencil;

	CObject* m_CubeObj;
	CObject* m_SummonCircle;

	CObject* m_Water;

	CObject* m_StoneGround;
	CObject* m_StoneAltar;

	CObject* m_menuObj;

	CObject* m_buttonBack;
	CObject* m_button1;
	CObject* m_button2;

	TextLabel* m_menutext;
};

#endif // !__SCENEMANAGER_H__
