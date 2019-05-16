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
	void BuildIndexBuffer();
	void LoadHeightMap();

	float GetHeight(float x, float z) const;
	void Smooth();
	bool InBounds(UINT _a, UINT _b);
	float Average(UINT _a, UINT _b);
	void Render();

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

	std::string m_strFilePath;
	float m_fHeightScale;
	float m_fHeightOffset;
	float m_fWidth;
	float m_fDepth;

};

