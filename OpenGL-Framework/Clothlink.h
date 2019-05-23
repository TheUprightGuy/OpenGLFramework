#pragma once
#include "utils.h"
#include "ClothParticle.h"
class CClothLink
{
public:
	CClothLink(CClothParticle* _point1, CClothParticle* _point2, float restingDist, float stiff, float tearSensitivity)
		:m_p1(_point1), m_p2(_point2),
		m_fRestingDistance(restingDist),
		m_fStiffness(stiff), m_fTearSensitivity(tearSensitivity)
	{}

	// Solve the link constraint
	void solve() {
		// calculate the distance between the two PointMasss

		glm::vec3 diff = m_p1->m_vPos - m_p2->m_vPos;
		float d = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

		// find the difference, or the ratio of how far along the restingDistance the actual distance is.
		float difference = (m_fRestingDistance - d) / d;

		// if the distance is more than curtainTearSensitivity, the cloth tears
		if (d > m_fTearSensitivity)
		{
			//p1.removeLink(this);
		}

		// Inverse the mass quantities
		float im1 = 1 / m_p1->m_fMass;
		float im2 = 1 / m_p2->m_fMass;
		float scalarP1 = (im1 / (im1 + im2)) * m_fStiffness;
		float scalarP2 = m_fStiffness - scalarP1;

		// Push/pull based on mass
		// heavier objects will be pushed/pulled less than attached light objects
		m_p1->m_vPos.x += diff.x * scalarP1 * difference;
		m_p1->m_vPos.y += diff.y * scalarP1 * difference;
		m_p1->m_vPos.z += diff.z * scalarP1 * difference;

		m_p2->m_vPos.x -= diff.x * scalarP2 * difference;
		m_p2->m_vPos.y -= diff.y * scalarP2 * difference;
		m_p2->m_vPos.z -= diff.z * scalarP2 * difference;
	}

	void Render();

	~CClothLink();

private:
	float m_fRestingDistance;
	float m_fStiffness;
	float m_fTearSensitivity;

	CClothParticle* m_p1;
	CClothParticle* m_p2;
};