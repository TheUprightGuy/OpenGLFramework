#include "SceneManager.h"

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
	m_program = CProgrammerManager::GetInstance().GetProgram(DEFAULT);

	/*Main OBJ*/
	initinfo objInfo;
	objInfo.imgFilepath = "Resources/egg.png";
	objInfo.objPosition = glm::vec3(0.0f, 2.0f, 0.0f);
	m_CubeObj = new CObject(DEFAULT, MESH_CUBE, objInfo);


	/*Water*/
	initinfo waterInfo;
	waterInfo.objScale = glm::vec3(15.0f, 0.0f, 15.0f);
	waterInfo.objPosition = glm::vec3(0.0f, -0.3f, 0.0f);
	waterInfo.imgFilepath = "Resources/Textures/water.jpg";
	waterInfo.alpha = 0.7f;
	m_Water = new CObject(DEFAULT, MESH_CUBE, waterInfo);

	/*Ground*/
	initinfo stoneInfo;
	stoneInfo.objScale = glm::vec3(15.0f, 0.0f, 15.0f);
	stoneInfo.objPosition = glm::vec3(0.0f, -0.7f, 0.0f);
	stoneInfo.imgFilepath = "Resources/Textures/stone.jpg";
	m_StoneGround = new CObject(DEFAULT, MESH_CUBE, stoneInfo);

	/*Altar*/
	stoneInfo.objScale = glm::vec3(4.0f, 1.0f, 4.0f);
	stoneInfo.objPosition = glm::vec3(0.0f, -0.1f, 0.0f);
	m_StoneAltar = new CObject(DEFAULT, MESH_CUBE, stoneInfo);
	m_Stencil = new CStencilObj(m_StoneAltar);

	/*Summon Circle*/
	initinfo summonInfo;
	summonInfo.imgFilepath = "Resources/summoningcircle.png";
	summonInfo.objPosition = glm::vec3(0.0f, 1.0f, 0.0f);
	summonInfo.objScale = glm::vec3(3.5f, 0.0f, 3.5f);
	m_SummonCircle = new CObject(DEFAULT, MESH_CUBE, summonInfo);

	/*Menus and buttons*/
	m_menutext = new TextLabel("Press Q for control list", "Resources/Fonts/arial.ttf", glm::vec2(132.0f, 72.0f));
	m_menutext->SetScale(0.05f);

	initinfo menuInfo;
	menuInfo.objScale = glm::vec3(0.21f, 0.7f, 1.0f);
	menuInfo.objPosition = glm::vec3(1.5f , 0.0f, 0.0f);
	menuInfo.imgFilepath = "Resources/menu.png";
	m_menuObj = new CObject(DEFAULT, MESH_2D_SPRITE, menuInfo);

	initinfo backinfo;
	backinfo.objScale = glm::vec3(0.05f, 0.09f, 0.01f);
	backinfo.imgFilepath = "Resources/crate.jpg";
	m_buttonBack = new CObject(DEFAULT, MESH_CUBE, backinfo);

	initinfo buttonInfo;
	buttonInfo.objScale = glm::vec3(0.02f, 0.02f, 0.01f);
	buttonInfo.imgFilepath = "Resources/green.jpg";
	m_button1 = new CObject(DEFAULT, MESH_CUBE, buttonInfo);

	buttonInfo.imgFilepath = "Resources/red.jpg";
	m_button2 = new CObject(DEFAULT, MESH_CUBE, buttonInfo);
	/***********************************************************/

	/*Camera Setup*/
	CCameraManager::GetInstance().GetCam()->CamTarget(m_CubeObj->GetPos() - CCameraManager::GetInstance().GetCam()->GetCamPos());
	CCameraManager::GetInstance().GetCam()->CamTranslate(glm::vec3(0.0f, 3.0f, 8.0f));
}

void CSceneManager::Render()
{
	static bool lIsPressed;
	static bool lState;

	/*Stencil testing toggle. Never fixed the stencil testing :( */
	if ((((CInput::GetInstance().GetKeyState('l') == INPUT_HOLD) || (CInput::GetInstance().GetKeyState('L') == INPUT_HOLD)) && !lIsPressed))
	{
		lIsPressed = true;
		lState = !lState;
	}
	if ((CInput::GetInstance().GetKeyState('l') == INPUT_RELEASE) || (CInput::GetInstance().GetKeyState('L') == INPUT_RELEASE))
	{
		lIsPressed = false;
	}

	if (lState)
	{
		m_Stencil->Render();
	}
	else
	{
		m_StoneAltar->Render(CCameraManager::GetInstance().GetCam());
	}


	m_StoneGround->Render(CCameraManager::GetInstance().GetCam());
	m_Water->Render(CCameraManager::GetInstance().GetCam());

	m_SummonCircle->Render(CCameraManager::GetInstance().GetCam());
	m_CubeObj->Render(CCameraManager::GetInstance().GetCam());

	
	m_menutext->Render();
	m_menuObj->Render(CCameraManager::GetInstance().GetOrthoCam());
	
	m_buttonBack->Render(CCameraManager::GetInstance().GetCam());
	m_button1->Render(CCameraManager::GetInstance().GetCam());
	m_button2->Render(CCameraManager::GetInstance().GetCam());
}

void CSceneManager::Process()
{	
	//MenuHandling
	/***********************************************************************/
	static bool menuState = false;
	static bool KeyIsPressed;
	static float menuPerc = 1.0f;

	if ((((CInput::GetInstance().GetKeyState('q') == INPUT_HOLD) || (CInput::GetInstance().GetKeyState('Q') == INPUT_HOLD))
		&& !KeyIsPressed))
	{
		KeyIsPressed = true;
		menuState = !menuState;
		menuPerc = 0.0f;
	}
	if ((CInput::GetInstance().GetKeyState('q') == INPUT_RELEASE) || (CInput::GetInstance().GetKeyState('Q') == INPUT_RELEASE))
	{
		KeyIsPressed = false;
	}

	if (menuState)
	{
		m_menuObj->Translate(lerpfunc(glm::vec3(1.5f, 0.0f, 0.0f), glm::vec3(0.8f, 0.0f, 0.0f), menuPerc));
	}
	else
	{
		m_menuObj->Translate(lerpfunc(glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(1.5f, 0.0f, 0.0f), menuPerc));
	}

	if (menuPerc < 1.0f)
	{
		menuPerc += 0.2f;
	}

	static float thecagepercent = 0.0f;
	if (thecagepercent < 1.0f)
	{
		m_CubeObj->Translate(lerpfunc(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), thecagepercent)); //Raise up the cage
		CCameraManager::GetInstance().GetCam()->CamTarget(m_CubeObj->GetPos() - CCameraManager::GetInstance().GetCam()->GetCamPos()); //Camera following
		thecagepercent += 0.01f;
	}

	if (((CInput::GetInstance().GetKeyState('w') == INPUT_HOLD || (CInput::GetInstance().GetKeyState('W') == INPUT_HOLD)) //KeyPress Case
		|| (CheckMouseCollision(m_button1) && CInput::GetInstance().GetMouseState(0) == INPUT_HOLD)) //On screen button press case
		&& (CCameraManager::GetInstance().GetCam()->GetCamPos().z >= 2.0f)) //Keep camera within bounds
	{
		glm::vec3 camPos = CCameraManager::GetInstance().GetCam()->GetCamPos();
		camPos.z -= 0.5f;
		CCameraManager::GetInstance().GetCam()->CamTranslate(camPos);

		m_button1->Alpha(0.5f);
	}
	else
	{
		m_button1->Alpha(1.0f);
	}
	if (((CInput::GetInstance().GetKeyState('s') == INPUT_HOLD || (CInput::GetInstance().GetKeyState('S') == INPUT_HOLD))//KeyPress Case
		|| (CheckMouseCollision(m_button2) && CInput::GetInstance().GetMouseState(0) == INPUT_HOLD))//On screen button press case
		&& (CCameraManager::GetInstance().GetCam()->GetCamPos().z <= 30.0f)) //Keep camera within bounds
	{
		glm::vec3 camPos = CCameraManager::GetInstance().GetCam()->GetCamPos();
		camPos.z += 0.5f;
		CCameraManager::GetInstance().GetCam()->CamTranslate(camPos);

		m_button2->Alpha(0.5f);
	}
	else
	{
		m_button2->Alpha(1.0f);
	}
	
	//ROTATE
	static float rotDegrees = 0.0f;
	rotDegrees += 1.0;
	if (rotDegrees == 360.0f)
	{
		rotDegrees = 0.0f;
	}
	m_CubeObj->Rotation(rotDegrees, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 camPos = CCameraManager::GetInstance().GetCam()->GetCamPos();
	camPos.z -= 0.45f;
	camPos.y -= 0.08f;
	camPos.x -= 0.35f;
	m_buttonBack->Translate(camPos);

	camPos.z += 0.08f;
	camPos.y += 0.05f;
	camPos.x += 0.05f;
	m_button1->Translate(camPos);

	camPos.y -= 0.07f;
	m_button2->Translate(camPos);
}

//Takes a plane and point and evals position in regards to the plane
PointPlace CSceneManager::EvalPoint(Plane PlaneToEval, glm::vec3 PointPos)
{
	float fDistance = glm::dot(PlaneToEval.PlaneNorm, PlaneToEval.PlanePoint); 
	float fFinalVal = (glm::dot(PlaneToEval.PlaneNorm, PointPos)) - fDistance;

	if (fFinalVal == 0)
	{
		return(ON_PLANE);
	}
	else if (fFinalVal > 0)
	{
		return(FRONT_OF_PLANE);
	}
	else if (fFinalVal < 0)
	{
		return(BACK_OF_PLANE);
	}
}

bool CSceneManager::EvalLine(Plane PlaneToEval, Line LineToEval, glm::vec3& CollisionPoint)
{

	float LinePlaneScalar = (glm::dot(PlaneToEval.PlaneNorm, (PlaneToEval.PlanePoint - LineToEval.P1)) / glm::dot(PlaneToEval.PlaneNorm, (LineToEval.P2 - LineToEval.P1)));


	CollisionPoint = LineToEval.P1 + (LinePlaneScalar * (LineToEval.P2 - LineToEval.P1));

	if (LinePlaneScalar > 0 && LinePlaneScalar < 1)
	{
		return (true);
	}

	return false;
}

bool CSceneManager::CheckMouseCollision(CObject* _object)
{
	Line lineEval;
	lineEval.P1 = CCameraManager::GetInstance().GetCam()->GetCamPos();
	lineEval.P2 = CCameraManager::GetInstance().GetCam()->GetCamPos() + (10.0f * CInput::GetInstance().GetLookDirection());
	Plane planeEval;
	planeEval.PlanePoint = { _object->GetPos().x,
		_object->GetPos().y,
		_object->GetPos().z + (_object->GetScale().z) };

	planeEval.PlaneNorm = glm::normalize(_object->GetPos() - planeEval.PlanePoint);

	glm::vec3 collisionPoint;

	if (EvalLine(planeEval, lineEval, collisionPoint))
	{
		float fbottom = _object->GetPos().y - (_object->GetScale().y);
		float ftop = _object->GetPos().y + (_object->GetScale().y);

		float fleft = _object->GetPos().x - (_object->GetScale().x);
		float fRight = _object->GetPos().x + (_object->GetScale().x);

		if (collisionPoint.x > fleft && collisionPoint.x < fRight &&
			collisionPoint.y > fbottom && collisionPoint.y < ftop)
		{
			return (true);
		}

	}
	return(false);
}
  

