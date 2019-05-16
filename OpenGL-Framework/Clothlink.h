#pragma once
#include "utils.h"
#include "ClothParticle.h"
class CClothLink
{
public:
	CClothLink();
	~CClothLink();

private:
	float _fRestingDistance;
	float _fStiffness;
	float _fTearSensitivity;

	CClothParticle m_p1;
	CClothParticle m_p2;
};