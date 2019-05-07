#pragma once
#include "utils.h"

struct PARTICLE
{
	glm::vec3 position;
	glm::vec3 velocity;
} typedef ClothParticle;

class CCloth
{
public:
	CCloth(int _width, int _length);
	~CCloth();

	//Start Pos @ the top left of the cloth
	void init(glm::vec3 _startPos);
private:
	std::vector<ClothParticle*> m_points;
	
	int m_width;
	int m_length;

};

