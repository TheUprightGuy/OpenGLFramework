#include "SceneManager.h"
#include  <math.h>

CSceneManager* CSceneManager::s_pSceneManager = nullptr;

CSceneManager::CSceneManager()
{
}


CSceneManager::~CSceneManager()
{
}

CSceneManager& CSceneManager::GetInstance()
{
	if (s_pSceneManager == nullptr)
	{
		s_pSceneManager = new CSceneManager();
	}

	return(*s_pSceneManager);
}

void CSceneManager::DestroyInstance()
{
	delete s_pSceneManager;
	s_pSceneManager = nullptr;
}

void CSceneManager::Init()
{
	m_program = CProgrammerManager::GetInstance().GetProgram(PHONGLIGHTING);

	oldDeltaTime = 1.0f;

	/*Menus and buttons*/
	m_menutext = new TextLabel("Press Q for control list", "Resources/Fonts/arial.ttf", glm::vec2(132.0f, 72.0f));
	m_menutext->SetScale(0.6f);
	m_menutext->SetPos({ 127.0f, 1.0f });
	m_menutext->SetColor({ 0.0f, 0.0f, 0.0f });



	initinfo menuInfo;
	menuInfo.objScale = glm::vec3(0.25f, 0.8f, 1.0f);
	menuInfo.objPosition = glm::vec3(1.5f , 0.0f, 0.0f);
	menuInfo.imgFilepath = "Resources/menu.png";
	m_menuObj = new CObject(DEFAULT, MESH_2D_SPRITE, menuInfo);

	m_fGravity = -0.98f;
	m_fRestDis = 10.0f;
	m_ConstAcc = 3.0f;
	m_Mass = 1.0f;

	m_cloth = new CCloth(m_fRestDis,  m_ConstAcc, m_fGravity, m_Mass);
	//m_cloth = new CCloth();
	m_cloth->init();

	m_slider = new CSlider(0.6f, -1.47f, -1.27f, -1.07f);
	/*Camera Setup*/
	menuInfo.objScale = glm::vec3(0.25f, 0.8f, 1.0f);
	menuInfo.objPosition = glm::vec3(-1.5f, 0.0f, 0.0f);
	menuInfo.imgFilepath = "Resources/box.png";
	m_slidersBG = new CObject(DEFAULT, MESH_2D_SPRITE, menuInfo);

	m_GravText = new TextLabel(std::to_string(m_fGravity), "Resources/Fonts/arial.ttf", glm::vec2(132.0f, 72.0f));
	m_GravText->SetScale(0.6f);
	float xConverted = (m_slider->GetSliderPosX()) * (float)glutGet(GLUT_WINDOW_WIDTH);
	float yConverted = (0.65) * (float)glutGet(GLUT_WINDOW_HEIGHT);
	//m_GravText->SetPos({ xConverted , yConverted });
	m_GravText->SetColor({ 1.0f, 0.0f, 0.0f });
}

void CSceneManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);

	m_slider->render();
	m_menutext->Render();
	

	m_menuObj->Render(CCameraManager::GetInstance().GetOrthoCam());
	m_slidersBG->Render(CCameraManager::GetInstance().GetOrthoCam());
	m_cloth->render();

	m_GravText->Render();

	glutSwapBuffers();
}

void CSceneManager::Process()
{	
	float newFrameTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	float deltaTime = newFrameTime - oldDeltaTime;
	oldDeltaTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	if (deltaTime < 1.0f)
	{
		deltaTime = 1.0f;
	}
	m_cloth->process(deltaTime);


	

	//MenuHandling
	/***********************************************************************/
	static bool menuState = false;
	static float menuPerc = 1.0f;

	if ((CInput::GetInstance().GetKeyState('e') == INPUT_FIRST_PRESS) || (CInput::GetInstance().GetKeyState('E') == INPUT_FIRST_PRESS))
	{
		menuState = !menuState;
		menuPerc = 0.0f;
	}

	if (menuState)
	{
		m_menuObj->Translate(lerpfunc(glm::vec3(1.5f, 0.0f, 0.0f), glm::vec3(0.725f, 0.0f, 0.0f), menuPerc));
		
	}
	else
	{
		m_menuObj->Translate(lerpfunc(glm::vec3(0.725f, 0.0f, 0.0f), glm::vec3(1.5f, 0.0f, 0.0f), menuPerc));
	}

	if (menuPerc < 1.0f)
	{
		
		menuPerc += 0.2f;
	}

	static bool slidersState = false;
	static float slidersPerc = 1.0f;

	if ((CInput::GetInstance().GetKeyState('q') == INPUT_FIRST_PRESS) || (CInput::GetInstance().GetKeyState('Q') == INPUT_FIRST_PRESS) && 
		slidersPerc >= 1.0f)
	{
		slidersState = !slidersState;
		slidersPerc = 0.0f;
	}

	if (slidersState)
	{
		m_slidersBG->Translate(lerpfunc(glm::vec3(-1.5f, 0.0f, 0.0f), glm::vec3(-0.725f, 0.0f, 0.0f), slidersPerc));
	}
	else
	{
		m_slidersBG->Translate(lerpfunc(glm::vec3(-0.725f, 0.0f, 0.0f), glm::vec3(-1.5f, 0.0f, 0.0f), slidersPerc));
	}
	if (slidersPerc < 1.0f)
	{
		if (slidersState)
		{
			m_slider->Translate(0.11, 0.11, 0.11);
		}
		else
		{
		
			m_slider->Translate(-0.11, -0.11, -0.11);
		}
		
		slidersPerc += 0.2f;
	}
	//MenuHandling
	/***********************************************************************/
	
	static bool bDebug = false;
	static float yaw = 0.0f;
	static float pitch = 0.0f;

	if ((CInput::GetInstance().GetSpecialKeyState(GLUT_KEY_F4) == INPUT_FIRST_PRESS))
	{
		m_Controls = MOUSEFREE;
	}
	if ((CInput::GetInstance().GetSpecialKeyState(GLUT_KEY_F3) == INPUT_FIRST_PRESS))
	{
		m_Controls = FREECAM;
	}
	if ((CInput::GetInstance().GetSpecialKeyState(GLUT_KEY_F2) == INPUT_FIRST_PRESS))
	{
		m_Controls = THIRDPERSON;
	}
	
	switch (m_Controls)
	{
	case FIRSTPERSON:
		break;
	case THIRDPERSON:
		break;
	case FREECAM:
		CCameraManager::GetInstance().DebugCamera(deltaTime, 50.0f);
		break;
	case MOUSEFREE:
		glutSetCursor(GLUT_CURSOR_INHERIT);
		break;
	default:
		break;
	}

}

  

