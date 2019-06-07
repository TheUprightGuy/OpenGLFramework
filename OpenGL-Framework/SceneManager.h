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
#include "ShaderLoader.h"
#include "AudioManager.h"
#include "CCloth.h"
#include "CSlider.h"

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

	float m_Mass;
	float m_fGravity;
	float m_fDamping;
	int m_fRestDis;
	int m_ConstAcc;
	float m_fStiffness;
	float m_fTearSensitivity;
	
	//Variables
	GLuint m_program;

	ControlScheme m_Controls;
	float oldDeltaTime;

	CCloth* m_cloth;

	
	CObject* m_slidersBG;

	CObject* m_menuObj;
	TextLabel* m_menutext;

	CSlider* m_gravSlider;
	TextLabel* m_GravText;
	CSlider* m_massSlider;
	TextLabel* m_MassText;
	CSlider* m_stiffSlider;
	TextLabel* m_stiffText;
	CSlider* m_restDisSlider;
	TextLabel* m_restDisText;
	CSlider* m_tearSlider;
	TextLabel* m_tearText;
	CSlider* m_dampSlider;
	TextLabel* m_dampText;

};

#endif // !__SCENEMANAGER_H__

