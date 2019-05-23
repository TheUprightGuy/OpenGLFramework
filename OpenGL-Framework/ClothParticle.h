#pragma once
#include "utils.h"
#include "Clothlink.h"

class CClothParticle
{
public:
	CClothParticle(glm::vec3 _initPos)
	{
		m_vPos = _initPos;
		m_vLastPos = _initPos;
		m_vAcc = glm::vec3();
	}

	void applyForce(glm::vec3 _forceVec) {
		m_vAcc += (m_vPos / m_fMass);
	}
	void pinTo(glm::vec3 _pinnedPos) {
		pinned = true;
		m_vPinnedPos = _pinnedPos;
	}

	void updatePhysics(float timeStep) { // timeStep should be in elapsed seconds (deltaTime)
		applyForce({0.0f, m_fMass * m_fGravity, 0.0f});

		glm::vec3 velocity = m_vPos - m_vLastPos;
		// dampen velocity
		velocity *= 0.99;

		float timeStepSq = timeStep * timeStep;

		glm::vec3 m_nextPos;
		// calculate the next position using Verlet Integration
		m_nextPos.x = m_vPos.x + velocity.x + 0.5 * m_vAcc.x * timeStepSq;
		m_nextPos.y = m_vPos.y + velocity.y + 0.5 * m_vAcc.y * timeStepSq;
		m_nextPos.z = m_vPos.z + velocity.z + 0.5 * m_vAcc.z * timeStepSq;
		// reset variables
		m_vLastPos = m_vPos;
		
		m_vAcc = glm::vec3();
	}

	~CClothParticle();

private:
	float m_fMass = 1;
	float m_fDamping = 20;
	float m_fGravity = 980.0f;

	glm::vec3 m_vPos;
	glm::vec3 m_vLastPos;
	glm::vec3 m_vAcc;

	std::vector<CClothLink*> m_links;

	bool pinned = false;
	glm::vec3 m_vPinnedPos;
};