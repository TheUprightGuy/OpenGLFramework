#pragma once
#include "utils.h"
#include "GeoModel.h"

class CParticle : public CGeometryModel
{
public:
	CParticle(GLuint program, glm::vec3 _origin, glm::vec3 _vel, float _elapsed, int _id)
	: CGeometryModel(program),
		m_origin(_origin), m_velocity(_vel), m_elapsedTime(_elapsed), m_id(_id)
	{
		this->m_position = this->m_origin;
	}
	
	void update(float dt)
	{
		this->m_velocity.y += -0.2 * .0167f;
		this->m_position += m_velocity;
		this->m_elapsedTime -= 0.000167f;

		if (this->m_elapsedTime <= 0.0f)
		{
			this->m_position = this->m_origin;
			this->m_velocity =
				glm::vec3(
					0.25 * cos(this->m_id * .0167) + 0.25f * util::randomFloat() - 0.125f,
					1.5f + 0.25f * util::randomFloat() - 0.125f,
					0.25 * sin(this->m_id* .0167) + 0.25f * util::randomFloat() - 0.125f);
			this->m_elapsedTime = util::randomFloat() + 0.125;
		}
	}


	~CParticle();

private:
	glm::vec3 m_velocity;
	glm::vec3 m_origin;
	
	float m_elapsedTime;
	int m_id;
};