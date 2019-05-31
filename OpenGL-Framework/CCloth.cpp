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
			CClothParticle *newParticle = new CClothParticle({x * m_iRestingDistances, y * m_iRestingDistances + m_yStart, 0.0f});

			if (x != 0)
			{
				//pointmass.attachTo((PointMass)(pointmasses.get(pointmasses.size() - 1)), restingDistances, stiffnesses);
				CClothLink* newLink = new CClothLink(newParticle, m_particles[m_particles.size() - 1], m_iRestingDistances, 1.0f, 0.7f);
				m_links.push_back(newLink);
			}

			// the index for the PointMasss are one dimensions, 
			// so we convert x,y coordinates to 1 dimension using the formula y*width+x  
			if (y != 0)
			{
				//pointmass.attachTo((PointMass)(pointmasses.get((y - 1) * (m_width + 1) + x)), m_iRestingDistances, 1);
				CClothLink* newLink = new CClothLink(newParticle, m_particles[(y - 1) * (m_width + 1) + x], m_iRestingDistances, 1.0f, 0.7f);
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
}

void CCloth::render()
{
	glUseProgram(CProgrammerManager::GetInstance().GetProgram(PHONGLIGHTING));

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
	GLint ModelLoc = glGetUniformLocation(CProgrammerManager::GetInstance().GetProgram(PHONGLIGHTING), "model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	glm::mat4 MVP;
	/********************************///MVP

	glm::mat4 view = CCameraManager::GetInstance().GetCam()->GetView();
	glm::mat4 proj = CCameraManager::GetInstance().GetCam()->GetProj();
	glUniform3fv(glGetUniformLocation(CProgrammerManager::GetInstance().GetProgram(PHONGLIGHTING), "camPos"), 1, glm::value_ptr(CCameraManager::GetInstance().GetCam()->GetCamPos()));

	MVP = proj * view * Model;

	GLint MVPLoc = glGetUniformLocation(CProgrammerManager::GetInstance().GetProgram(PHONGLIGHTING), "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	/***********************************/


	glUniform1f(glGetUniformLocation(CProgrammerManager::GetInstance().GetProgram(PHONGLIGHTING), "alpha"), 1.0f);
	/***********************************/


	for (auto x : m_links)
	{
		x->Render();
	}
}

void CCloth::process(float timestep)
{
	for (int i = 0; i < m_iConstraintAccuracy; i++)
	{
		for (auto x : m_links)
		{
			x->solve();
		}
	}
	

	for (auto x : m_particles)
	{
		x->updatePhysics(timestep);
	}

	for (auto x : m_particles)
	{
		float fRand = (rand() % 20 + 0) * 0.1f;
		x->applyForce({ 0.0f, 0.0f, x->m_fMass * fRand});
	}
	

}

