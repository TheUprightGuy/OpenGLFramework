#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GLuint _program, glm::vec3 origin, std::string texFileName)
	: program(_program)
{
	nParticles = 4000;

	for (int i = 0; i < nParticles; i++)
	{
		vPosition.push_back(glm::vec3(0.0)); //initialize position vector

		CParticle* p = new CParticle
		( 
			program,
			origin, // pos
			glm::vec3(0.25 * cos(i * .0167) + 0.25f * util::randomFloat() - 0.125f, // vel
				2.0f + 0.25f * util::randomFloat() - 0.125f,
				0.25 * sin(i* .0167) + 0.25f * util::randomFloat() - 0.125f),

			util::randomFloat() + 0.125, // elapsed time
			i		);	}
}
