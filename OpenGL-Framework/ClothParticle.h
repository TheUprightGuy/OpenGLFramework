#pragma once
#include "utils.h"
#include "Clothlink.h"

class CClothParticle
{
public:
	CClothParticle();
	~CClothParticle();

private:
	float lastX, lastY;
	float x, y;
	float accX, accY;

	std::vector<CClothLink*> m_links;

	bool pinned = false;
	float pinX, pinY;
};