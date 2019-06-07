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

	menuInfo.objScale = glm::vec3(0.25f, 0.8f, 1.0f);
	menuInfo.objPosition = glm::vec3(-1.5f, 0.0f, 0.0f);
	menuInfo.imgFilepath = "Resources/box.png";

	m_slidersBG = new CObject(DEFAULT, MESH_2D_SPRITE, menuInfo);

	float xConverted = (-1.2) * (float)glutGet(GLUT_WINDOW_WIDTH);
	float yConverted = (0.65) * (float)glutGet(GLUT_WINDOW_HEIGHT);

	m_GravText = new TextLabel(std::to_string(m_fGravity), "Resources/Fonts/arial.ttf", glm::vec2(xConverted, yConverted));
	m_GravText->SetScale(0.6f);
	m_GravText->SetColor({ 0.0f, 0.652f, 0.928f });
	m_gravSlider = new CSlider(0.6f, -1.485f, -1.3f, -1.18f);

	xConverted = (-1.2) * (float)glutGet(GLUT_WINDOW_WIDTH);
	yConverted = (0.65) * (float)glutGet(GLUT_WINDOW_HEIGHT);
	m_MassText = new TextLabel(std::to_string(m_fGravity), "Resources/Fonts/arial.ttf", glm::vec2(xConverted, yConverted));
	m_MassText->SetScale(0.6f);
	m_MassText->SetColor({ 0.0f, 0.652f, 0.928f });
	m_massSlider = new CSlider(0.4f, -1.485f, -1.3f, -1.18f);

	xConverted = (-1.2) * (float)glutGet(GLUT_WINDOW_WIDTH);
	yConverted = (0.65) * (float)glutGet(GLUT_WINDOW_HEIGHT);
	m_stiffText = new TextLabel(std::to_string(m_fGravity), "Resources/Fonts/arial.ttf", glm::vec2(xConverted, yConverted));
	m_stiffText->SetScale(0.6f);
	m_stiffText->SetColor({ 0.0f, 0.652f, 0.928f });
	m_stiffSlider = new CSlider(0.2f, -1.485f, -1.3f, -1.18f);

	xConverted = (-1.2) * (float)glutGet(GLUT_WINDOW_WIDTH);
	yConverted = (0.65) * (float)glutGet(GLUT_WINDOW_HEIGHT);
	m_restDisText = new TextLabel(std::to_string(m_fGravity), "Resources/Fonts/arial.ttf", glm::vec2(xConverted, yConverted));
	m_restDisText->SetScale(0.6f);
	m_restDisText->SetColor({ 0.0f, 0.652f, 0.928f });
	m_restDisSlider = new CSlider(0.0f, -1.485f, -1.35f, -1.18f);

	xConverted = (-1.2) * (float)glutGet(GLUT_WINDOW_WIDTH);
	yConverted = (0.65) * (float)glutGet(GLUT_WINDOW_HEIGHT);
	m_tearText = new TextLabel(std::to_string(m_fGravity), "Resources/Fonts/arial.ttf", glm::vec2(xConverted, yConverted));
	m_tearText->SetScale(0.6f);
	m_tearText->SetColor({ 0.0f, 0.652f, 0.928f });
	m_tearSlider = new CSlider(-0.2f, -1.485f, -1.35f, -1.18f);

	xConverted = (-1.2) * (float)glutGet(GLUT_WINDOW_WIDTH);
	yConverted = (0.65) * (float)glutGet(GLUT_WINDOW_HEIGHT);
	m_dampText = new TextLabel(std::to_string(m_fGravity), "Resources/Fonts/arial.ttf", glm::vec2(xConverted, yConverted));
	m_dampText->SetScale(0.6f);
	m_dampText->SetColor({ 0.0f, 0.652f, 0.928f });
	m_dampSlider = new CSlider(-0.4f, -1.485f, -1.3f, -1.18f);

}

void CSceneManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);

	m_dampSlider->render();
	m_tearSlider->render();
	m_restDisSlider->render();
	m_stiffSlider->render();
	m_massSlider->render();
	m_gravSlider->render();

	m_slidersBG->Render(CCameraManager::GetInstance().GetOrthoCam());
	m_menuObj->Render(CCameraManager::GetInstance().GetOrthoCam());

	m_cloth->render();
	m_GravText->Render();	
	m_MassText->Render();
	m_stiffText->Render();
	m_restDisText->Render();
	m_tearText->Render();	
	m_dampText->Render();
		
	m_menutext->Render();

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

	
	m_fGravity = -m_gravSlider->process();
	std::string tempText = std::to_string(m_fGravity);
	tempText.erase(6, tempText.find_last_of("0"));
	m_GravText->SetText(tempText);

	m_fStiffness = m_stiffSlider->process();
	tempText = std::to_string(m_fStiffness);
	tempText.erase(6, tempText.find_last_of("0"));
	m_stiffText->SetText(tempText);

	m_Mass = m_massSlider->process();
	tempText = std::to_string(m_Mass);
	tempText.erase(6, tempText.find_last_of("0"));
	m_MassText->SetText(tempText);

	m_fRestDis = m_restDisSlider->process() * 25;
	tempText = std::to_string(m_fRestDis);
	//tempText.erase(6, tempText.size() - 1);
	m_restDisText->SetText(tempText);

	m_fTearSensitivity = m_tearSlider->process() * 100.0f;
	tempText = std::to_string(m_fTearSensitivity);
	tempText.erase(4, tempText.size() - 1);
	m_tearText->SetText(tempText);

	m_fDamping = m_dampSlider->process();
	tempText = std::to_string(m_fDamping);
	tempText.erase(6, tempText.find_last_of("0"));
	m_dampText->SetText(tempText);


	if (slidersState)
	{
		m_slidersBG->Translate(lerpfunc(glm::vec3(-1.5f, 0.0f, 0.0f), glm::vec3(-0.725f, 0.0f, 0.0f), slidersPerc));

		glm::vec3 textPos = lerpfunc(glm::vec3(-1.2f, 0.97f, 0.0f), glm::vec3(-0.64f, 0.97f, 0.0f), slidersPerc);
		float xConverted = (textPos.x + 1) * (float)glutGet(GLUT_WINDOW_WIDTH);
		float yConverted = ((textPos.y / 2) + 1) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_GravText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-1.2f, 0.59f, 0.0f), glm::vec3(-0.64f, 0.59f, 0.0f), slidersPerc);
		xConverted = (textPos.x + 1) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = ((textPos.y / 2) + 1) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_MassText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-1.2f, 0.22f, 0.0f), glm::vec3(-0.64f, 0.22f, 0.0f), slidersPerc);
		xConverted = (textPos.x + 1) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = ((textPos.y / 2) + 1) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_stiffText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-1.2f, -0.15f, 0.0f), glm::vec3(-0.64f, -0.15f, 0.0f), slidersPerc);
		xConverted = (textPos.x + 1) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = ((textPos.y / 2) + 1) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_restDisText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-1.2f, -0.52f, 0.0f), glm::vec3(-0.64f, -0.52f, 0.0f), slidersPerc);
		xConverted = (textPos.x + 1) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = ((textPos.y / 2) + 1) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_tearText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-1.2f, -0.89f, 0.0f), glm::vec3(-0.64f, -0.89f, 0.0f), slidersPerc);
		xConverted = (textPos.x + 1) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = ((textPos.y / 2) + 1) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_dampText->SetPos({ xConverted , yConverted });
	}
	else
	{
		m_slidersBG->Translate(lerpfunc(glm::vec3(-0.725f, 0.0f, 0.0f), glm::vec3(-1.5f, 0.0f, 0.0f), slidersPerc));

		glm::vec3 textPos = lerpfunc(glm::vec3(-0.64f, 0.97f, 0.0f), glm::vec3(-1.2f, 0.97f, 0.0f), slidersPerc);
		float xConverted = (textPos.x) * (float)glutGet(GLUT_WINDOW_WIDTH);
		float yConverted = (textPos.y) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_GravText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-0.64f, 0.57f, 0.0f), glm::vec3(-1.2f, 0.57f, 0.0f), slidersPerc);
		xConverted = (textPos.x) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = (textPos.y) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_MassText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-0.64f, 0.22f, 0.0f), glm::vec3(-1.2f, 0.22f, 0.0f), slidersPerc);
		xConverted = (textPos.x) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = (textPos.y) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_stiffText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-0.64f, -0.15f, 0.0f), glm::vec3(-1.2f, -0.15f, 0.0f), slidersPerc);
		xConverted = (textPos.x) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = (textPos.y) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_restDisText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-0.64f, -0.52f, 0.0f), glm::vec3(-1.2f, -0.52f, 0.0f), slidersPerc);
		xConverted = (textPos.x) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = (textPos.y) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_tearText->SetPos({ xConverted , yConverted });

		textPos = lerpfunc(glm::vec3(-0.64f, -0.89f, 0.0f), glm::vec3(-1.2f, -0.89f, 0.0f), slidersPerc);
		xConverted = (textPos.x) * (float)glutGet(GLUT_WINDOW_WIDTH);
		yConverted = (textPos.y) * (float)glutGet(GLUT_WINDOW_HEIGHT);
		m_dampText->SetPos({ xConverted , yConverted });
	}

	if (slidersPerc < 1.0f)
	{
		if (slidersState)
		{
			m_gravSlider->Translate(0.11, 0.11, 0.11);
			m_massSlider->Translate(0.11, 0.11, 0.11);
			m_stiffSlider->Translate(0.11, 0.11, 0.11);
			m_restDisSlider->Translate(0.11, 0.11, 0.11);
			m_tearSlider->Translate(0.11, 0.11, 0.11);
			m_dampSlider->Translate(0.11, 0.11, 0.11);
		}
		else
		{
		
			m_gravSlider->Translate(-0.11, -0.11, -0.11);
			m_massSlider->Translate(-0.11, -0.11, -0.11);
			m_stiffSlider->Translate(-0.11, -0.11, -0.11);
			m_restDisSlider->Translate(-0.11, -0.11, -0.11);
			m_tearSlider->Translate(-0.11, -0.11, -0.11);
			m_dampSlider->Translate(-0.11, -0.11, -0.11);
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

  

