#pragma once
#include "utils.h"
#include "ClothParticle.h"
#include "Clothlink.h"

class CCloth
{
public:
	CCloth(int _width, int _length);
	~CCloth();

	//Start Pos @ the top left of the cloth
	void init(glm::vec3 _startPos);

private:
	int m_width;
	int m_length;
	float gravity;

	std::vector<CClothParticle*> m_particles;
};

