#pragma once
#include "utils.h"
#include "ClothParticle.h"
#include "Clothlink.h"
#include "CameraManager.h"
#include "ProgramManager.h"
#include "object.h"

class CCloth
{
public:
	/*CCloth()
	{
		m_length = 20;
		m_width = 20;
		m_yStart = 1;
		m_iRestingDistances = 10.0f;
		m_iConstraintAccuracy = 3;
		m_fMass = 0.2f;
		m_fDamp = 0.98;
		m_Gravity = -0.98;

		ShaderLoader _shader;
		m_program = _shader.CreateProgram("Shaders/ClothVertexShader.vs", "Shaders/ClothFragmentShader.fs");
	}*/

	CCloth(int& _restingDist, int& _ConstraintAcc, float& gravity, float& _mass, float _damp = 0.99f)
		:m_iRestingDistances(_restingDist ), m_iConstraintAccuracy(_ConstraintAcc), m_Gravity(gravity), m_fMass(_mass), m_fDamp(_damp)
	{
		m_length = 20;
		m_width = 20;
		m_yStart = 1;

		ShaderLoader _shader;
		m_program = _shader.CreateProgram("Shaders/ClothVertexShader.vs", "Shaders/ClothFragmentShader.fs");
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

	int& m_iRestingDistances;

	float& m_Gravity;
	float& m_fMass;
	float m_fDamp;

	GLuint m_program;
	CObject* m_sphere;
	CObject* m_ground;
	int& m_iConstraintAccuracy;


	std::vector<CClothParticle*> m_particles;
	std::vector<CClothLink*> m_links;
};

