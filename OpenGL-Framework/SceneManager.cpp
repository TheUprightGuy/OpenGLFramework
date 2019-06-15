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
	m_Controls = MOUSEFREE;

	/*Menus and buttons*/
	m_menutext = new TextLabel("Press Q for menu", "Resources/Fonts/arial.ttf", glm::vec2(132.0f, 72.0f));
	m_menutext->SetScale(0.06f);
	m_menutext->SetPos({ 132.0f, 5.0f });
	m_menutext->SetColor({ 0.0f, 0.0f, 0.0f });

	initinfo menuInfo;
	menuInfo.objScale = glm::vec3(0.21f, 0.7f, 1.0f);
	menuInfo.objPosition = glm::vec3(1.5f , 0.0f, 0.0f);
	menuInfo.imgFilepath = "Resources/menu.png";
	m_menuObj = new CObject(DEFAULT, MESH_2D_SPRITE, menuInfo);

	initinfo cubeInfo;
	cubeInfo.objScale = glm::vec3(1.0f, 1.0f, 1.0f);
	cubeInfo.objPosition = glm::vec3( 193.574112, -13.8693628, 196.926041 );
	cubeInfo.imgFilepath = "Resources/LOKI.jpg";
	cubeInfo.RotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	cubeInfo.RotationDegrees = 90.0f;
	m_Cube = new CObject(DEFAULT, MESH_CUBE, cubeInfo);


	m_terrain = new Terrain();
	m_terrain->Initialize();
	/*Camera Setup*/

	m_light = new CLight(m_program);
	m_light->Position({ 50.0f, 1000.0f, 0.0f });

	m_model = new Model("Resources/Models/skull/skull.obj");

	 
}

void CSceneManager::Render()
{
	/*if (m_frameBuffer != nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer->GetFBO());
		glEnable(GL_DEPTH_TEST);
	}*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0);
	
	m_model->Render();

	m_light->Render();
	m_terrain->Render();

	//m_model->Render();
	//m_Cube->Render(CCameraManager::GetInstance().GetCam());

	//m_menutext->Render();
	//m_menuObj->Render(CCameraManager::GetInstance().GetOrthoCam());

	//if (m_frameBuffer != nullptr)
	//{
	//	m_frameBuffer->Render();
	//}

	glutSwapBuffers();
}

void CSceneManager::Process()
{	
	float newFrameTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	float deltaTime = newFrameTime - oldDeltaTime;
	oldDeltaTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	//MenuHandling
	/***********************************************************************/
	static bool menuState = false;
	static float menuPerc = 1.0f;

	if ((CInput::GetInstance().GetKeyState('q') == INPUT_FIRST_PRESS) || (CInput::GetInstance().GetKeyState('Q') == INPUT_FIRST_PRESS))
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

	//MenuHandling
	/***********************************************************************/
	
	static bool bDebug = false;
	static float yaw = 0.0f;
	static float pitch = 0.0f;
	static int seed = 789187;
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
	if ((CInput::GetInstance().GetSpecialKeyState(GLUT_KEY_F5) == INPUT_FIRST_PRESS))
	{
		m_terrain->RebuildTerrain();
	}
	if ((CInput::GetInstance().GetSpecialKeyState(GLUT_KEY_F6) == INPUT_FIRST_PRESS))
	{
		seed = (rand() % 70000)/* + 70000*/;
		printf("%d \n", seed);

		m_terrain->setSeed(seed);
		
	}
	switch (m_Controls)
	{
	case FIRSTPERSON:
		break;
	case THIRDPERSON:
		PlayerControls();
		break;
	case FREECAM:
		CCameraManager::GetInstance().DebugCamera(deltaTime, 100.0f);
		break;
	case MOUSEFREE:
		glutSetCursor(GLUT_CURSOR_INHERIT);
		break;
	default:
		break;
	}


}

void CSceneManager::PlayerControls()
{
	glm::vec3 PlayerPos = m_Cube->GetPos();
	float rads = glm::radians(m_Cube->GetRot());

	//Moves the player according to its current rotation
	/*****************************************************************/
	if (CInput::GetInstance().GetKeyState('w') == INPUT_HOLD)
	{
		glm::vec3 forwardVec = glm::vec3(glm::cos(-rads), 0.0, glm::sin(-rads)); //I'm gonna be honest, I don't understand why this works, but it does, so I'm happy with it
		PlayerPos = PlayerPos + forwardVec;
	}

	if (CInput::GetInstance().GetKeyState('s') == INPUT_HOLD)
	{
		glm::vec3 backVec = glm::vec3(glm::cos(-rads), 0.0, glm::sin(-rads));
		PlayerPos = PlayerPos - backVec;
	}

	if (CInput::GetInstance().GetKeyState('a') == INPUT_HOLD)
	{
		glm::vec3 leftVec = glm::vec3(glm::cos(-glm::radians(m_Cube->GetRot() - 90)), 0.0, glm::sin(-glm::radians(m_Cube->GetRot() - 90)));
		PlayerPos = PlayerPos - leftVec;
	}

	if (CInput::GetInstance().GetKeyState('d') == INPUT_HOLD)
	{
		glm::vec3 rightVec = glm::vec3(glm::cos(-glm::radians(m_Cube->GetRot() + 90)), 0.0, glm::sin(-glm::radians(m_Cube->GetRot() + 90)));
		PlayerPos = PlayerPos - rightVec;
	}
	PlayerPos.y = m_terrain->GetHeight(PlayerPos.x, PlayerPos.z) + (m_Cube->GetScale().y);

	float fGive = 2.0f;
	if ((PlayerPos.x > -(250.0f + fGive) && PlayerPos.x < (250.0f + fGive)) && (PlayerPos.z > -(250.0f + fGive) && PlayerPos.z < (250.0f + fGive)))
	{
		m_Cube->Translate(PlayerPos);
	}


	//Deadzone size is 2
	if (CInput::GetInstance().GetMousePos().x < glutGet(GLUT_WINDOW_WIDTH) / 2 + 2)
	{
		m_Cube->Rotation(m_Cube->GetRot() + 7, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (CInput::GetInstance().GetMousePos().x > glutGet(GLUT_WINDOW_WIDTH) / 2 - 2)
	{
		m_Cube->Rotation(m_Cube->GetRot() - 7, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	glutSetCursor(GLUT_CURSOR_NONE);

	glm::vec3 camVec = glm::vec3(glm::cos(-glm::radians(m_Cube->GetRot())), -0.7, glm::sin(-glm::radians(m_Cube->GetRot())));
	CCameraManager::GetInstance().GetCam()->CamTarget(m_Cube->GetPos());
	CCameraManager::GetInstance().GetCam()->SetCamPos(m_Cube->GetPos() + (-camVec * 8.0f));
	CCameraManager::GetInstance().GetCam()->UpdateView();
}

