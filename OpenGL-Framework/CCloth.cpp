#include "CCloth.h"
#include <cstdlib>
#include <time.h>

void CCloth::init()
{
	srand(time(NULL));

	for (int y = 0; y <= m_length; y++)
	{
		for (int x = 0; x <= m_width; x++) 
		{
			CClothParticle *newParticle = new CClothParticle({x * m_iRestingDistances, y * m_iRestingDistances + m_yStart, 0.0f}, m_fMass, m_Gravity);

			if (x != 0)
			{
				CClothLink* newLink = new CClothLink(newParticle, m_particles[m_particles.size() - 1], m_iRestingDistances, m_fStiffNess, m_fTearSensitivity);
				m_links.push_back(newLink);
			}

			// the index for the PointMasss are one dimensions, 
			// so we convert x,y coordinates to 1 dimension using the formula y*width+x  
			if (y != 0)
			{
				CClothLink* newLink = new CClothLink(newParticle, m_particles[(y - 1) * (m_width + 1) + x], m_iRestingDistances, m_fStiffNess, m_fTearSensitivity);
				m_links.push_back(newLink);
			}

			// we pin the very top PointMasss to where they are
			if (y == m_length)
			{
				newParticle->pinTo(newParticle->m_vPos);
			}

	
			// add to PointMass array  
			m_particles.push_back(newParticle);
		}
	}

	initinfo sphereInfo;
	sphereInfo.color = { 0.0f, 1.0f, 0.0f };
	sphereInfo.objScale = {20.0f, 20.0f , 20.0f };
	//m_sphere = new CObject(DEFAULT, MESH_SPHERE, sphereInfo);

	initinfo groundInfo;
	//groundInfo.color = { 0.0f, 1.0f, 0.0f };
	groundInfo.imgFilepath = "Resources/green.jpg";
	groundInfo.objScale = { 200.0f, 1.0f , 100.0f };
	groundInfo.objPosition = { 50.0f, -100.0f, 0.0f };
	m_ground = new CObject(DEFAULT, MESH_CUBE, groundInfo);
}

void CCloth::render()
{
	m_ground->Render(CCameraManager::GetInstance().GetCam());
	//m_sphere->Render(CCameraManager::GetInstance().GetCam());
	glUseProgram(m_program);

	//glPolygonMode(GL_FRONT, GL_LINE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Translate matrix
	glm::mat4 translate = glm::translate(glm::mat4(), { 0.0f, 0.0f, 0.0f });

	/************************************///Rotation Matrix
	glm::vec3 rotAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(0.0f), { 0.0f, 0.0f, 1.0f });

	/************************************///Scale matrix
	glm::mat4 scale = glm::scale(glm::mat4(), { 1.0f, 1.0f, 1.0f });

	/************************************///Texture Scale matrix

	glm::mat4 Model = translate * rotation * scale;
	GLint ModelLoc = glGetUniformLocation(m_program, "model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	glm::mat4 MVP;
	/********************************///MVP

	glm::mat4 view = CCameraManager::GetInstance().GetCam()->GetView();
	glm::mat4 proj = CCameraManager::GetInstance().GetCam()->GetProj();

	MVP = proj * view * Model;

	GLint MVPLoc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	/***********************************/

	/***********************************/

	for (auto x : m_links)
	{
		glUniform3fv(glGetUniformLocation(m_program, "setColor"), 1, glm::value_ptr(x->lerpColor));
		x->Render();
	}

}

void CCloth::process(float timestep, bool _mouseForces)
{
	static CClothParticle* m_Collided;
	glm::vec3 lookVec = CInput::GetInstance().GetLookDirection();

	//Identify the particle the mouse colides with by getting the distance from
	//the camera to the particle then checking that distance along the look vector
	//to see if the particle is within the radius of this point.
	//If the above is true, the output is put into m_collided
	/***************************************************************/
	for (auto x : m_particles)
	{
		CCamera* baseCam = CCameraManager::GetInstance().GetCam();

		float camToX = glm::distance(x->m_vPos, baseCam->GetCamPos());
		glm::vec3 check = lookVec * camToX;
		check += baseCam->GetCamPos();

		if (glm::distance(check, x->m_vPos) < 5.0f)
		{
			m_Collided = x;
		}

		glm::vec3 pos = x->m_vPos;
		glm::vec3 groundPos = m_ground->GetPos();
		glm::vec3 groundScale = m_ground->GetScale();

		if (pos.y < groundPos.y + groundScale.y)
		{ 
			x->m_vPos.y = m_ground->GetPos().y + 0.2f;
		}
		else
		{
			x->m_fUpdatePhysics = true;
		}

	}
	/***************************************************************/

	//This changes the amount of force excerted on m_collided when the mouse is clicked.
	/***************************************************************/
	static float force = 10.0f;
	if ((CInput::GetInstance().GetKeyState(',') == INPUT_HOLD || CInput::GetInstance().GetKeyState('<') == INPUT_HOLD )
		&& force >= -75)
	{
		force -= 5.0f;
		std::cout << force << std::endl;
	}
	if ((CInput::GetInstance().GetKeyState('.') == INPUT_HOLD || CInput::GetInstance().GetKeyState('>') == INPUT_HOLD) 
		&& force <= 75)
	{
		force += 5.0f;
		std::cout << force << std::endl;

	}
	//This applies said force
	/***************************************************************/
	if (CInput::GetInstance().GetMouseState(0) == INPUT_HOLD && m_Collided != nullptr && _mouseForces)
	{
		m_Collided->applyForce(lookVec * m_fForce);
	}
	/***************************************************************/

	//Below checks if the middle mouse is clicked than m_collided is put into
	//m_locked and kept until middle mouse is released.
	/***************************************************************/
	static CClothParticle* m_Locked = nullptr;
	static bool bLocked = false;

	if (CInput::GetInstance().GetMouseState(1) == INPUT_HOLD && !bLocked && _mouseForces)
	{
		m_Locked = m_Collided;
		bLocked = true;
	}
	else if (CInput::GetInstance().GetMouseState(1) == INPUT_RELEASE)
	{
		bLocked = false;
	}

	//If middle click is held down then lock m_locked position to the mouse position,
	//or more accurately the mouse position in regards to the cloth
	/***************************************************************/
	if (bLocked )
	{
		CCamera* baseCam = CCameraManager::GetInstance().GetCam();

		float camToX = glm::distance(m_Locked->m_vPos, baseCam->GetCamPos());
		glm::vec3 check = lookVec * camToX;
		check += baseCam->GetCamPos();

		m_Locked->m_vPos = check;
	}

	//This takes does the same check as m_collided, except it does it
	//for both points in a link, and if both points are colliding then
	//the link is deleted and removed from the array.
	/***************************************************************/
	int iCount = 0;
	for (auto x : m_links)
	{
		CCamera* baseCam = CCameraManager::GetInstance().GetCam();
		CClothParticle* p1 = x->GetP1();
		CClothParticle* p2 = x->GetP2();

		float camToX = glm::distance(p1->m_vPos, baseCam->GetCamPos());
		glm::vec3 check1 = lookVec * camToX;
		check1 += baseCam->GetCamPos();

		camToX = glm::distance(p2->m_vPos, baseCam->GetCamPos());
		glm::vec3 check2 = lookVec * camToX;
		check2 += baseCam->GetCamPos();

		float fPointDistance = glm::distance(p1->m_vPos, p2->m_vPos);
		if (glm::distance(check1, p1->m_vPos) < fPointDistance && glm::distance(check2, p2->m_vPos) < fPointDistance)
		{
			if (CInput::GetInstance().GetMouseState(2) == INPUT_HOLD && _mouseForces)
			{
				m_links.erase(m_links.begin() + iCount);
			}
		}

		iCount++;
	}

	//Solves the physics and distance for links, and if the distance
	//is further than the tear sensitivity then the link is removed.
	/***************************************************************/
	int iLinkCount = 0;
	for (int i = 0; i < m_iConstraintAccuracy; i++)
	{
		iLinkCount = 0;

		for (auto x : m_links)
		{
			if (x->solve())
			{
				//m_links.erase(m_links.begin() + iLinkCount);
				std::cout << "You're tearing me apart Lisa number  " << iLinkCount << std::endl;
				m_links.erase(m_links.begin() + iLinkCount);
				bLocked = false;
			}

			iLinkCount++;
		}
	}

	//Update the physics on each point.
	/***************************************************************/
	for (auto x : m_particles)
	{
		if (x->m_fUpdatePhysics)
		{
		x->updatePhysics(timestep);
		}
	}
	
}

