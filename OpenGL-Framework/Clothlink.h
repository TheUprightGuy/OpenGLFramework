#pragma once
#include "utils.h"
#include "ProgramManager.h"
#include "ClothParticle.h"
#include <math.h>


class CClothLink
{
public:
	CClothLink(CClothParticle* _point1, CClothParticle* _point2, float restingDist, float stiff, float tearSensitivity)
		:m_p1(_point1), m_p2(_point2),
		m_fRestingDistance(restingDist),
		lerpColor({1.0f, 0.0f, 0.0f}),
		m_fStiffness(stiff), m_fTearSensitivity(tearSensitivity)
	{}

	// Solve the link constraint
	bool solve() 
	{
		// calculate the distance between the two PointMasss
		glm::vec3 diff =m_p1->m_vPos - m_p2->m_vPos;
		float d = std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

		float lerpy = d- m_fRestingDistance / m_fTearSensitivity- m_fRestingDistance;
		glm::vec3 vecA = { 0.0f, 1.0f, 0.0f };
		glm::vec3 vecB = { 1.0f, 0.0f, 0.0f };

		lerpColor = vecA + lerpy * (vecB - vecA);

		float difference = (m_fRestingDistance - d) / d;

		float im1 = 1 / m_p1->m_fMass;
		float im2 = 1 / m_p2->m_fMass;
		float scalarP1 = (im1 / (im1 + im2)) * m_fStiffness;
		float scalarP2 = m_fStiffness - scalarP1;

	
		m_p1->m_vPos.x += diff.x * scalarP1 * difference;
		m_p1->m_vPos.y += diff.y * scalarP1 * difference;
		m_p1->m_vPos.z += diff.z * scalarP1 * difference;

		m_p2->m_vPos.x -= diff.x * scalarP2 * difference;
		m_p2->m_vPos.y -= diff.y * scalarP2 * difference;
		m_p2->m_vPos.z -= diff.z * scalarP2 * difference;

		if (d > m_fTearSensitivity)
		{
			return true;
		}

		return false;

	}

	void Render()
	{
		//glUniform3fv(glGetUniformLocation(CProgrammerManager::GetInstance().GetProgram(DEFAULT), "setColor"), 1, glm::value_ptr(lerpColor));

		glBegin(GL_LINES);
		glColor3f(lerpColor.x, lerpColor.y, lerpColor.z);
		glVertex3f(m_p1->m_vPos.x, m_p1->m_vPos.y, m_p1->m_vPos.z);
		glVertex3f(m_p2->m_vPos.x, m_p2->m_vPos.y, m_p2->m_vPos.z);
		glEnd();
	}

	CClothParticle* GetP1() { return(m_p1); }
	CClothParticle* GetP2() { return(m_p2); }
	~CClothLink();

	glm::vec3 lerpColor;
private:
	CClothParticle* m_p2;
	CClothParticle* m_p1;
	
	float m_fRestingDistance;
	float m_fStiffness;
	float m_fTearSensitivity;

};