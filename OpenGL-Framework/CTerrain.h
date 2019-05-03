#pragma once
#include "utils.h"
#include "CameraManager.h"
#include "ProgramManager.h"
struct TerrainVertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texC;
};

struct TerrainInfo
{
	std::wstring HeightmapFilename;
	float HeightScale;
	float HeightOffset;
	UINT NumRows;
	UINT NumCols;
	float CellSpacing;
};

class CTerrain
{
public:
	CTerrain();
	CTerrain(TerrainInfo& initInfo);

	void initTerrain();
	void Render();
	
	~CTerrain();
private:

	GLuint m_program;

	// Get the heightmap from the RAW file
	void loadHeightMap();

	// Compute the average height of IJ
	float average(int i, int j);

	//Smooth using this ^
	void smooth();

	bool inBounds(UINT i, UINT j);

	std::vector<float> m_Heightmap;
	TerrainInfo m_Info;

	GLuint m_ebo;
	GLuint m_vao;
	GLuint m_vbo;

	std::vector<TerrainVertex> m_vertices;
	std::vector<int> m_indices;

	void GenVB();
	void GenIB();

	GLuint IndiceCount;


	GLuint m_texture;
	bool m_isTextured;

	//Translate
	glm::vec3 m_objPosition;
	//COORD m_objPixelPos;

	//Scale
	glm::vec3 m_objScale;

	//Rotation
	float m_RotationDegrees;
	glm::vec3 m_RotAxis;

	//Texture mapping
	glm::vec2 m_texStartScale;
	glm::vec2 m_texEndScale;

	std::vector<glm::vec3> m_vecNormals;
	//color
	glm::vec3 m_color;

	float m_Alpha;
};

