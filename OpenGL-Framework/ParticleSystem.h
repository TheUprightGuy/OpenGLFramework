#pragma once
#include "Particle.h"
#include "CameraManager.h"
#include "utils.h"

class ParticleSystem 
{
public:
	ParticleSystem(GLuint _program, glm::vec3 origin, std::string texFileName);

	~ParticleSystem();

	void render(float dt);

	std::vector<CParticle*> particles;
	std::vector<glm::vec3> vPosition;

private:
	GLuint vao, vbo, texture, program;
	float nParticles;
};