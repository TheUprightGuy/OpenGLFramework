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
	m_Controls = THIRDPERSON;

	/*Menus and buttons*/
	m_menutext = new TextLabel("Press Q for control list", "Resources/Fonts/arial.ttf", glm::vec2(132.0f, 72.0f));
	m_menutext->SetScale(0.06f);
	m_menutext->SetPos({ 127.0f, 1.0f });
	m_menutext->SetColor({ 0.0f, 0.0f, 0.0f });

	initinfo menuInfo;
	menuInfo.objScale = glm::vec3(0.21f, 0.7f, 1.0f);
	menuInfo.objPosition = glm::vec3(1.5f , 0.0f, 0.0f);
	menuInfo.imgFilepath = "Resources/menu.png";
	m_menuObj = new CObject(DEFAULT, MESH_2D_SPRITE, menuInfo);



	/*Camera Setup*/

}

void CSceneManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0);
	
	m_menutext->Render();
	m_menuObj->Render(CCameraManager::GetInstance().GetOrthoCam());

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
  

