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
#include "terrain.h"
#include "GeoModel.h"
#include "TessModel.h"
#include "FrameBuffer.h"

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

enum ControlScheme
{
	FIRSTPERSON,
	THIRDPERSON,
	FREECAM,
	MOUSEFREE
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

	void PlayerControls();
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

	ControlScheme m_Controls;
	float oldDeltaTime;
	//Global input
	CInput* m_pInput;

	CObject* m_menuObj;
	CObject* m_Cube;
	Model* m_model;

	TextLabel* m_menutext;

	Terrain* m_terrain;

	CLight* m_light;

	CGeometryModel* m_geoModel;
	CTessModel* m_tessModel;
	CFrameBuffer* m_frameBuffer;
};

#endif // !__SCENEMANAGER_H__

