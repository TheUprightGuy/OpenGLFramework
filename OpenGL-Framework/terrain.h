#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include <vector>
#include <string>
#include "light.h"

struct TerrainVertex
{
	glm::vec3 v3Pos;
	glm::vec3 v3Norm;
	glm::vec2 v2Tex;
};


class Terrain
{
public:
	Terrain();
	~Terrain();
	void Initialize();
	void BuildVertexBuffer();
	void RebuildVB();
	void BuildIndexBuffer();
	void LoadHeightMap();

	void RebuildTerrain()
	{
		LoadHeightMap();
		Smooth();

		RebuildVB();
	}

	float GetHeight(float x, float z) const;
	void Smooth();
	bool InBounds(UINT _a, UINT _b);
	float Average(UINT _a, UINT _b);

	void Render();

	void setSeed(int _seed) { seed = _seed; }

	float random(int x, int y, int fSeed)
	{
		int n = x + y * 57;
		n = (n << 13) ^ n;
		int t = (n * (n * n * 15731 + fSeed) + 1376312589) & 0x7fffffff;
		return 1.0 - double(t) * 0.931322574615478515625e-9;
	}

	float smooth(int x, int y)
	{
		float corners;
		float sides;
		float center;

		corners = (random(x - 1, y - 1, seed) + random(x + 1, y - 1, seed) + random(x - 1, y + 1, seed) + random(x + 1, y + 1, seed)) / 16;
		sides = (random(x - 1, y, seed) + random(x + 1, y, seed) + random(x, y - 1, seed) + random(x, y + 1, seed)) / 8;
		center = random(x, y, seed) / 4;
		return corners + sides + center;
	}

	float interpolate(float a, float b, float x)
	{
		/*return a * (1 - x) + b * x;*/

		float ft = x * 3.1415927; 
		float f = (1 - cos(ft)) * 0.5f;
		return a * (1 - f) + b * f;

	}

	float noise(float x, float y) 
	{
		float fractional_X = x - int(x);
		float fractional_Y = y - int(y);
		//smooths
		float v1 = smooth(int(x), int(y));
		float v2 = smooth(int(x) + 1, int(y));
		float v3 = smooth(int(x), int(y) + 1);
		float v4 = smooth(int(x) + 1, int(y) + 1);
		// interpolates
		float i1 = interpolate(v1, v2, fractional_X);
		float i2 = interpolate(v3, v4, fractional_X);
		return interpolate(i1, i2, fractional_Y);
	}

	float totalNoisePerPoint(int x, int y) 
	{
		int octaves = 8;
		float zoom = 800.0f;
		float persistence = 1.2f;
		float total = 0.0f;

		for (int i = 0; i < octaves - 1; i++) 
		{
			float frequency = pow(2, i) / zoom;
			float amplitude = pow(persistence, i);
			total += noise(x * frequency, y * frequency) * amplitude;
		}

		return total;
	}

private:
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_program;

	GLuint m_textureGrass;
	GLuint m_textureEarth;
	GLuint m_textureSnow;

	CLight* m_light;

	std::vector<TerrainVertex> m_vecVertices;
	std::vector<int> m_vecIndices;
	std::vector<float> m_vecHeightMap;
	std::vector<glm::vec3> m_vecNormals;
	unsigned int m_iNumIndices;

	unsigned int m_iNumCols;
	unsigned int m_iNumRows;
	int seed = 789187;
	std::string m_strFilePath;
	float m_fHeightScale;
	float m_fHeightOffset;
	float m_fWidth;
	float m_fDepth;
};

