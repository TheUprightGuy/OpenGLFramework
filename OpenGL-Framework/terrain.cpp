#include "terrain.h"
#include "utils.h"
#include "CameraManager.h"
#include "ProgramManager.h"
#include "Dependencies\soil\SOIL.h"
#include <fstream>
#include <iostream>

inline void DeleteTexture(GLuint& texID)
{
	if (texID != 0)
	{
		glDeleteTextures(1, &texID);
		texID = 0;
	}
}

inline void CreateTexture(GLuint& texID)
{
	DeleteTexture(texID);
	glGenTextures(1, &texID);
}

inline void DeleteVertexBuffer(GLuint& vboID)
{
	if (vboID != 0)
	{
		glDeleteBuffersARB(1, &vboID);
		vboID = 0;
	}
}


inline void CreateVertexBuffer(GLuint& vboID)
{
	// Make sure we don't loose the reference to the previous VBO if there is one
	DeleteVertexBuffer(vboID);
	glGenBuffersARB(1, &vboID);
}
Terrain::Terrain()
{
	m_iNumCols = 513;
	m_iNumRows = 513;
	m_fHeightScale = 0.35f;
	m_fHeightOffset = -20.0f;
	m_fWidth = static_cast<float>(m_iNumCols);
	m_fDepth = static_cast<float>(m_iNumRows);

	//m_strFilePath = "cliffs.raw";
}

Terrain::~Terrain()
{
}

void Terrain::Initialize()
{
	// Create program
	ShaderLoader shaderLoader;

	// m_program = CProgrammerManager::GetInstance().GetProgram(PHONGLIGHTING);
	m_program = shaderLoader.CreateProgram("Shaders/TerrainPhongLightingVS.vs", "Shaders/TerrainPhongLightingFS.fs");
		
	LoadHeightMap();
	Smooth();

	BuildVertexBuffer();
	BuildIndexBuffer();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(offsetof(TerrainVertex, v3Norm)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(offsetof(TerrainVertex, v2Tex)));
	glEnableVertexAttribArray(2);

	//Texture Loading
	/**************************************/
	glGenTextures(1, &m_textureGrass);
	glBindTexture(GL_TEXTURE_2D, m_textureGrass);

	int width, height;
	unsigned char* image = SOIL_load_image("Resources/Textures/grass.jpg", &width, &height, 0, SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &m_textureEarth);
	glBindTexture(GL_TEXTURE_2D, m_textureEarth);

	image = SOIL_load_image("Resources/Textures/earth.jpg", &width, &height, 0, SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &m_textureSnow);
	glBindTexture(GL_TEXTURE_2D, m_textureSnow);

	image = SOIL_load_image("Resources/Textures/snow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);
	/**************************************/

	m_light = new CLight(m_program);
	m_light->Position({ 50.0f, 500.0f, 0.0f });
}



void Terrain::BuildVertexBuffer()
{
	float halfWidth = (m_iNumCols - 1) * (0.5f * 1);
	float halfDepth = (m_iNumRows - 1) * (0.5f * 1);

	float du = 1.0f / (m_iNumCols - 1);
	float dv = 1.0f / (m_iNumRows - 1);
	//float du = 0.125f;
	//float dv = 0.125f;

	CONST int textureTilingNum = 8;

	int iXCounter = 0;
	int iYCounter = 0;

	std::vector<TerrainVertex> vertices((m_iNumCols * m_iNumRows));

	int CurrentIndex = 0;
	for (unsigned int i = 0; i < m_iNumRows; ++i)
	{
		float z = halfDepth - (i * 1);

		for (unsigned int j = 0; j < m_iNumCols; ++j)
		{
			float x = -halfWidth + j;
			float y = m_vecHeightMap[i * m_iNumCols + j];

			vertices[i * m_iNumCols + j].v3Pos = glm::vec3(x, y, z);

			vertices[i * m_iNumCols + j].v2Tex.x = j*du;
			vertices[i * m_iNumCols + j].v2Tex.y = i*dv;

			vertices[i * m_iNumCols + j].v3Norm = glm::vec3(0.0f, 1.0f, 0.0f);

			iYCounter++;
			if (iYCounter > textureTilingNum)
			{
				iYCounter = 0;
			}
		}

		iXCounter++;
		if (iXCounter > textureTilingNum)
		{
			iXCounter = 0;
		}
	}

	// Generate Normals		
	m_vecNormals.resize(m_iNumRows * m_iNumCols, glm::vec3());

	float invTwoDX = 1.0f / (2.0f * 1.0f); // 1.0f is cell seperation 
	float invTwoDZ = 1.0f / (2.0f * 1.0f);
	for (UINT i = 2; i < m_iNumRows - 1; ++i)
	{
		for (UINT j = 2; j < m_iNumCols - 1; ++j)
		{
			float t = m_vecHeightMap[(i - 1)* m_iNumCols + j];
			float b = m_vecHeightMap[(i + 1)* m_iNumCols + j];
			float l = m_vecHeightMap[i * m_iNumCols + j - 1];
			float r = m_vecHeightMap[i * m_iNumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b) * invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l) * invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			glm::normalize(N);
			vertices[i * m_iNumCols + j].v3Norm = N;
		}
	}

	m_vecVertices = vertices;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainVertex), &vertices[0], GL_STATIC_DRAW);
}

void Terrain::BuildIndexBuffer()
{
	std::vector<int> vecIndices(((m_iNumRows - 1) * (m_iNumCols - 1) * 2) * 3);
	//std::vector<int> vecIndices;
	unsigned int CurrentIndex = 0;

	for (unsigned int i = 0; i < m_iNumRows - 1; ++i)
	{
		for (unsigned int j = 0; j < m_iNumCols - 1; ++j)
		{
			vecIndices[CurrentIndex + 0] = i * m_iNumCols + j;
			vecIndices[CurrentIndex + 1] = i * m_iNumCols + j + 1;
			vecIndices[CurrentIndex + 2] = (i + 1) * m_iNumCols + j;

			vecIndices[CurrentIndex + 3] = (i + 1) * m_iNumCols + j;
			vecIndices[CurrentIndex + 4] = i * m_iNumCols + j + 1;
			vecIndices[CurrentIndex + 5] = (i + 1) * m_iNumCols + j + 1;

			CurrentIndex += 6; // next quad
		}
	}
	GLuint uiEBO;
	glGenBuffers(1, &uiEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecIndices.size() * sizeof(int), &vecIndices[0], GL_STATIC_DRAW);

	m_vecIndices = vecIndices;
}

void Terrain::LoadHeightMap()
{
	if (!m_strFilePath.empty())
	{
		// A height for each vertex
		std::vector<unsigned char> in(m_iNumRows * m_iNumCols);

		// Open the file.
		std::ifstream inFile;
		inFile.open(m_strFilePath.c_str(), std::ios_base::binary);

		if (inFile)
		{
			// Read the RAW bytes.
			inFile.read((char*)&in[0], (std::streamsize)in.size());

			// Done with file.
			inFile.close();
		}

		// Copy the array data into a float array, and scale and offset the heights.
		m_vecHeightMap.resize(m_iNumRows * m_iNumCols, 0);
		for (UINT i = 0; i < m_iNumRows * m_iNumCols; ++i)
		{
			m_vecHeightMap[i] = static_cast<float>(in[i]) * m_fHeightScale + m_fHeightOffset;
		}

	}
	else
	{
		for (int i = 0; i < m_iNumRows; i++)
		{
			for (int j = 0; j < m_iNumCols; j++)
			{
				m_vecHeightMap[(i - 1) * (m_iNumRows + 1) + j] = /* static_cast<float>(in[i]) **/ m_fHeightScale + m_fHeightOffset;
			}
		}
	}
	std::reverse(m_vecHeightMap.begin(), m_vecHeightMap.end());
}



float Terrain::GetHeight(float x, float z) const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*m_fWidth) / 1.0f;
	float d = (z - 0.5f*m_fDepth) / -1.0f;

	// Get the row and column we are in.
	int row = static_cast<int>(floorf(d));
	int col = static_cast<int>(floorf(c));

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_vecHeightMap[row*m_iNumCols + col];
	float B = m_vecHeightMap[row*m_iNumCols + col + 1];
	float C = m_vecHeightMap[(row + 1)*m_iNumCols + col];
	float D = m_vecHeightMap[(row + 1)*m_iNumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - static_cast<float>(col);
	float t = d - static_cast<float>(row);

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}

void Terrain::Smooth()
{
	std::vector<float> vecSmoothedHeightmap(m_vecHeightMap.size());

	for (UINT i = 0; i < m_iNumRows; ++i)
	{
		for (UINT j = 0; j < m_iNumCols; ++j)
		{
			vecSmoothedHeightmap[i * m_iNumCols + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_vecHeightMap = vecSmoothedHeightmap;
}

bool Terrain::InBounds(UINT _a, UINT _b)
{
	return
		(
		(_a >= 0 && _a < m_iNumRows) &&
			(_b >= 0 && _b < m_iNumCols)
			);
}

float Terrain::Average(UINT _a, UINT _b)
{
	// Function computes the average height of the AB element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |AB| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float fAvg = 0.0f;
	float fNum = 0.0f;

	for (UINT i = _a - 1; i <= _a + 1; ++i)
	{
		for (UINT j = _b - 1; j <= _b + 1; ++j)
		{
			if (InBounds(i, j))
			{
				fAvg += m_vecHeightMap[i * m_iNumCols + j];
				fNum += 1.0f;
			}
		}
	}

	return (fAvg * (1.0f / fNum));
}


void Terrain::Render()
{

	//// ** Render Normal Terrain ** ////
	// Set Culling and Use program	
	glUseProgram(m_program);

	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CCW); // Front face is Clockwise order

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_light->Render();
	
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, value_ptr(glm::mat4()));

	// Pass mvp to shader
	glm::mat4 MVP = CCameraManager::GetInstance().GetCam()->GetProj() * CCameraManager::GetInstance().GetCam()->GetView();
	GLuint  MVPloc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(MVPloc, 1, GL_FALSE, value_ptr(MVP));
	// Pass campos
	glm::vec3 camPos = CCameraManager::GetInstance().GetCam()->GetCamPos();

	//glUniform3fv(glGetUniformLocation(m_program, "camPos"), 1, value_ptr(camPos));

	//glUniform3fv(glGetUniformLocation(m_program, "setColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

	//glUniform1i(glGetUniformLocation(m_program, "isTextured"), 1);
	// Pass Tex

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureGrass);
	glUniform1i(glGetUniformLocation(m_program, "texGrass"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureEarth);
	glUniform1i(glGetUniformLocation(m_program, "texEarth"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_textureSnow);
	glUniform1i(glGetUniformLocation(m_program, "texSnow"), 2);

	// Bind vao and draw object, unbind vao
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_vecIndices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_PATCHES, 0, m_vecVertices.size());

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}
