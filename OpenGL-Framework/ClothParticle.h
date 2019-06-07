#pragma once
#include "utils.h"

class CClothParticle
{
public:
	CClothParticle(glm::vec3 _initPos, float& _mass, float& _gravity)
		:m_fMass(_mass), m_fGravity(_gravity)
	{
		m_vPos = _initPos;
		m_vLastPos = _initPos;
		m_vAcc = glm::vec3();
	}

	void applyForce(glm::vec3 _forceVec) 
	{
		m_vAcc += (_forceVec / m_fMass);
	}

	void pinTo(glm::vec3 _pinnedPos) 
	{
		pinned = true;
		m_vPinnedPos = _pinnedPos;
	}

	void updatePhysics(float timeStep) 
	{ 
		// timeStep should be in elapsed seconds (deltaTime)
		applyForce({0.0f, m_fMass * m_fGravity, 0.0f});
		static float prevMass = m_fMass;
		if (prevMass != m_fMass)
		{
			std::cout << m_fMass << std::endl;
			prevMass = m_fMass;
		}
		glm::vec3 velocity = m_vPos - m_vLastPos;
		// dampen velocity
		velocity *= m_fDamping;

		float timeStepSq = timeStep * timeStep;

		glm::vec3 m_nextPos;
		// calculate the next position using Verlet Integration
		m_nextPos.x = m_vPos.x + velocity.x + 0.5 * m_vAcc.x * timeStepSq;
		m_nextPos.y = m_vPos.y + velocity.y + 0.5 * m_vAcc.y * timeStepSq;
		m_nextPos.z = m_vPos.z + velocity.z + 0.5 * m_vAcc.z * timeStepSq;
		// reset variables
		m_vLastPos = m_vPos;

		if (pinned) 
		{
			m_vPos = m_vPinnedPos;
		}
		else
		{
			m_vPos = m_nextPos;
		}

		m_vAcc = glm::vec3();
	}

	

	~CClothParticle();

	glm::vec3 m_vPos;
	float& m_fMass;
	float m_fDamping = 0.99f;
	float& m_fGravity;
	bool m_fUpdatePhysics = true;

private:

	glm::vec3 m_vLastPos;
	glm::vec3 m_vAcc;

	bool pinned = false;
	glm::vec3 m_vPinnedPos;
};