#pragma once
#include "utils.h"
#include "ClothParticle.h"
#include "Clothlink.h"
#include "CameraManager.h"
#include "ProgramManager.h"
class CCloth
{
public:
	CCloth()
	{
		m_length = 20;
		m_width = 20;
		m_yStart = 1;
		m_iRestingDistances = 50.0f;
		m_iConstraintAccuracy = 5;
	}
	~CCloth();

	//Start Pos @ the top left of the cloth
	void init();
	void render();
	void process(float timestep);
private:
	int m_width;
	int m_length;

	int m_yStart;

	int m_iRestingDistances;
	float gravity;

	int m_iConstraintAccuracy;
	std::vector<CClothParticle*> m_particles;
	std::vector<CClothLink*> m_links;
};

