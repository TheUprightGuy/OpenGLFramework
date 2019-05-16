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
	m_menutext->SetScale(0.06f);
	m_menutext->SetPos({ 127.0f, 1.0f });
	initinfo menuInfo;
	menuInfo.objScale = glm::vec3(0.21f, 0.7f, 1.0f);
	menuInfo.objPosition = glm::vec3(1.5f , 0.0f, 0.0f);
	menuInfo.imgFilepath = "Resources/menu.png";
	m_menuObj = new CObject(DEFAULT, MESH_2D_SPRITE, menuInfo);

	initinfo cubeInfo;
	cubeInfo.objScale = glm::vec3(1.0f, 1.0f, 1.0f);
	cubeInfo.objPosition = glm::vec3( 193.574112, -13.8693628, 196.926041 );
	cubeInfo.imgFilepath = "Resources/NicCage.png";
	cubeInfo.RotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	cubeInfo.RotationDegrees = 90.0f;
	m_Cube = new CObject(DEFAULT, MESH_CUBE, cubeInfo);

	//m_model = new Model("Resources/Models/INGAME_BASE.OBJ");

	//m_headModel->Translate({ 0.0f, 50.0f, -10.0f });
	//TerrainInfo newTerrain;
	//newTerrain.HeightmapFilename = L"coastMountain513.raw";
	//newTerrain.HeightScale = 0.35f;
	//newTerrain.HeightOffset = -20.0f;
	//newTerrain.NumRows = 513;
	//newTerrain.NumCols = 513;
	//newTerrain.CellSpacing = 1.0f;


	m_terrain = new Terrain();
	m_terrain->Initialize();
	/*Camera Setup*/

	m_light = new CLight(m_program);
	m_light->Position({ 50.0f, 1000.0f, 0.0f });

	ShaderLoader GeoLoader;
	GLuint GeoProg = GeoLoader.CreateProgram("Shaders/GeoVertexShader.vs", "Shaders/GeoFragmentShader.fs", "Shaders/GeoGeometryShader.gs");

	m_geoModel = new CGeometryModel(GeoProg);
	m_geoModel->SetPosition({ 200.0f, -14.0f, 195.0f});

	GLuint tessProgram = GeoLoader.CreateProgram("Shaders/TessVertexShader.vs",
		"Shaders/TessFragmentShader.fs", nullptr,
		"Shaders/tessQuadModel.tcs",
		"Shaders/tessQuadModel.tes");

	m_tessModel = new CTessModel(tessProgram);
	m_tessModel->SetPos({ 190.0f, -14.0f, 195.0f });

	GLuint fbProgram = GeoLoader.CreateProgram("Shaders/FrameBufferVertex.vs",
		"Shaders/FrameBufferFragment.fs");

	m_frameBuffer = new CFrameBuffer(fbProgram);
}

void CSceneManager::Render()
{
	if (m_frameBuffer != nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer->GetFBO());
		glEnable(GL_DEPTH_TEST);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0);
	
	m_light->Render();
	m_terrain->Render();

	//m_model->Render();
	m_Cube->Render(CCameraManager::GetInstance().GetCam());

	
	m_geoModel->Render();

	m_tessModel->Render();

	m_menutext->Render();
	m_menuObj->Render(CCameraManager::GetInstance().GetOrthoCam());

	if (m_frameBuffer != nullptr)
	{
		m_frameBuffer->Render();
	}

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
	
	static bool debugKeyIsPressed;
	static bool bDebug = false;
	static float yaw = 0.0f;
	static float pitch = 0.0f;
	if ((CInput::GetInstance().GetSpecialKeyState(GLUT_KEY_F3) == INPUT_HOLD)
		&& !debugKeyIsPressed)
	{
		debugKeyIsPressed = true;
		bDebug = !bDebug;
	}
	if ((CInput::GetInstance().GetSpecialKeyState(GLUT_KEY_F3) == INPUT_RELEASE))
	{
		debugKeyIsPressed = false;
	}
	if (bDebug)
	{
		CCameraManager::GetInstance().DebugCamera(deltaTime, 100.0f);
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}

	if (!bDebug)
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
		m_Cube->Translate(PlayerPos);

		//Deadzone size is 2
		if (CInput::GetInstance().GetMousePos().x < glutGet(GLUT_WINDOW_WIDTH) / 2 + 2)
		{
			yaw += 7.0f;
			m_Cube->Rotation(m_Cube->GetRot() + 7, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (CInput::GetInstance().GetMousePos().x > glutGet(GLUT_WINDOW_WIDTH) / 2 - 2)
		{
			m_Cube->Rotation(m_Cube->GetRot() - 7, glm::vec3(0.0f, 1.0f, 0.0f));
			yaw -= 7.0f;
		}

		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
		glutSetCursor(GLUT_CURSOR_NONE);

		glm::vec3 camVec = glm::vec3(glm::cos(-glm::radians(m_Cube->GetRot())), -0.7, glm::sin(-glm::radians(m_Cube->GetRot())));
		CCameraManager::GetInstance().GetCam()->CamTarget(m_Cube->GetPos());
		CCameraManager::GetInstance().GetCam()->SetCamPos(m_Cube->GetPos() + (-camVec * 8.0f));
		CCameraManager::GetInstance().GetCam()->UpdateView();
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
  

