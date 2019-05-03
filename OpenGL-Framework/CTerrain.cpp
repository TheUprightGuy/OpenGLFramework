#include "CTerrain.h"
#include "ShaderLoader.h"
#include "utils.h"
#include "Camera.h"
#include "Dependencies\soil\SOIL.h"

#include <sstream>
#include <fstream>

CTerrain::CTerrain()
{
//	m_Info.NumCols = 513;
//	m_Info.NumRows = 513;
//	m_fHeightScale = 0.35f;
//	m_fHeightOffset = -20.0f;
//	m_fWidth = static_cast<float>(m_Info.NumCols);
//	m_fDepth = static_cast<float>(m_Info.NumRows);
//	m_strFilePath = "coastMountain513.raw";

}

CTerrain::CTerrain(TerrainInfo & initInfo)
	:m_Info(initInfo)
{
}

CTerrain::~CTerrain()
{
}

void CTerrain::initTerrain()
{
	// Create program
	m_program = CProgrammerManager::GetInstance().GetProgram(DEFAULT);

	loadHeightMap();
	smooth();
	GenVB();
	GenIB();

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(offsetof(TerrainVertex, texC)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(offsetof(TerrainVertex, normal)));
	glEnableVertexAttribArray(1);


	// Grass texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	int width, height;
	std::string texture = "ground.jpg";
	unsigned char* image = SOIL_load_image(
		texture.c_str(), // File path/name 
		&width, // Output for the image width
		&height, // Output for the image height
		0, // Output for number of channels
		SOIL_LOAD_RGBA); // Load RGBA values only
	glTexImage2D(
		GL_TEXTURE_2D, // Type of texture
		0, // Mipmap level, 0 for base
		GL_RGBA, // Number of color components in texture
		width, // Width of the texture
		height, // Height of the texture
		0, // This value must be 0
		GL_RGBA, // Format for the pixel data
		GL_UNSIGNED_BYTE, // Data type of the pixel data
		image); // Pointer to image data in memory
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


}

void CTerrain::GenVB()
{
	float halfWidth = (m_Info.NumCols - 1) * 0.5f;
	float halfDepth = (m_Info.NumRows - 1) * 0.5f;
	float du = 1.0f / (m_Info.NumCols - 1);
	float dv = 1.0f / (m_Info.NumRows - 1);

	std::vector<TerrainVertex> vertices((m_Info.NumCols * m_Info.NumRows));

	int CurrentIndex = 0;
	for (unsigned int i = 0; i < m_Info.NumRows; ++i)
	{
		float z = halfDepth - i;

		for (unsigned int j = 0; j < m_Info.NumCols; ++j)
		{
			float x = -halfWidth + j;
			float y = m_Heightmap[i * m_Info.NumCols + j];

			vertices[i * m_Info.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i * m_Info.NumCols + j].texC = glm::vec2(j*du, i*dv);
			vertices[i * m_Info.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}

	// Generate Normals		
	m_vecNormals.resize(m_Info.NumRows * m_Info.NumCols, glm::vec3());

	float invTwoDX = 1.0f / (2.0f * 1.0f); // 1.0f is cell seperation 
	float invTwoDZ = 1.0f / (2.0f * 1.0f);
	for (UINT i = 2; i < m_Info.NumRows - 1; ++i)
	{
		for (UINT j = 2; j < m_Info.NumCols - 1; ++j)
		{
			float t = m_Heightmap[(i - 1)* m_Info.NumCols + j];
			float b = m_Heightmap[(i + 1)* m_Info.NumCols + j];
			float l = m_Heightmap[i * m_Info.NumCols + j - 1];
			float r = m_Heightmap[i * m_Info.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b) * invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l) * invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			glm::normalize(N);
			vertices[i * m_Info.NumCols + j].normal = N;
		}
	}

	m_vertices = vertices;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainVertex), &vertices[0], GL_STATIC_DRAW);
}

void CTerrain::GenIB()
{
	std::vector<int> vecIndices(((m_Info.NumRows - 1) * (m_Info.NumCols - 1) * 2) * 3);
	//std::vector<int> vecIndices;
	unsigned int CurrentIndex = 0;

	for (unsigned int i = 0; i < m_Info.NumRows - 1; ++i)
	{
		for (unsigned int j = 0; j < m_Info.NumCols - 1; ++j)
		{
			vecIndices[CurrentIndex + 0] = i * m_Info.NumCols + j;
			vecIndices[CurrentIndex + 1] = i * m_Info.NumCols + j + 1;
			vecIndices[CurrentIndex + 2] = (i + 1) * m_Info.NumCols + j;

			vecIndices[CurrentIndex + 3] = (i + 1) * m_Info.NumCols + j;
			vecIndices[CurrentIndex + 4] = i * m_Info.NumCols + j + 1;
			vecIndices[CurrentIndex + 5] = (i + 1) * m_Info.NumCols + j + 1;

			CurrentIndex += 6; // next quad
		}
	}
	GLuint uiEBO;
	glGenBuffers(1, &uiEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecIndices.size() * sizeof(int), &vecIndices[0], GL_STATIC_DRAW);

	m_indices = vecIndices;
}

void CTerrain::loadHeightMap()
{
	// A height for each vertex
	std::vector<unsigned char> in(m_Info.NumRows * m_Info.NumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(m_Info.HeightmapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	m_Heightmap.resize(m_Info.NumRows * m_Info.NumCols, 0);
	for (UINT i = 0; i < m_Info.NumRows * m_Info.NumCols; ++i)
	{
		m_Heightmap[i] = static_cast<float>(in[i]) * m_Info.HeightScale + m_Info.HeightOffset;
	}

	std::reverse(m_Heightmap.begin(), m_Heightmap.end());
}


//float Terrain::GetHeight(float x, float z) const
//{
//	// Transform from terrain local space to "cell" space.
//	float c = (x + 0.5f*m_fWidth) / 1.0f;
//	float d = (z - 0.5f*m_fDepth) / -1.0f;
//
//	// Get the row and column we are in.
//	int row = static_cast<int>(floorf(d));
//	int col = static_cast<int>(floorf(c));
//
//	// Grab the heights of the cell we are in.
//	// A*--*B
//	//  | /|
//	//  |/ |
//	// C*--*D
//	float A = m_vecHeightMap[row*m_Info.NumCols + col];
//	float B = m_vecHeightMap[row*m_Info.NumCols + col + 1];
//	float C = m_vecHeightMap[(row + 1)*m_Info.NumCols + col];
//	float D = m_vecHeightMap[(row + 1)*m_Info.NumCols + col + 1];
//
//	// Where we are relative to the cell.
//	float s = c - static_cast<float>(col);
//	float t = d - static_cast<float>(row);
//
//	// If upper triangle ABC.
//	if (s + t <= 1.0f)
//	{
//		float uy = B - A;
//		float vy = C - A;
//		return A + s * uy + t * vy;
//	}
//	else // lower triangle DCB.
//	{
//		float uy = C - D;
//		float vy = B - D;
//		return D + (1.0f - s)*uy + (1.0f - t)*vy;
//	}
//}

void CTerrain::smooth()
{
	std::vector<float> vecSmoothedHeightmap(m_Heightmap.size());

	for (UINT i = 0; i < m_Info.NumRows; ++i)
	{
		for (UINT j = 0; j < m_Info.NumCols; ++j)
		{
			vecSmoothedHeightmap[i * m_Info.NumCols + j] = average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_Heightmap = vecSmoothedHeightmap;
}

bool CTerrain::inBounds(UINT _a, UINT _b)
{
	return
		(
		(_a >= 0 && _a < m_Info.NumRows) &&
			(_b >= 0 && _b < m_Info.NumCols)
			);
}

float CTerrain::average(int _a, int _b)
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
			if (inBounds(i, j))
			{
				fAvg += m_Heightmap[i * m_Info.NumCols + j];
				fNum += 1.0f;
			}
		}
	}

	return (fAvg * (1.0f / fNum));
}

void CTerrain::Render()
{

	glUseProgram(m_program);

	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CW); // Front face is Clockwise order

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	//glPolygonMode(GL_FRONT, GL_LINE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Translate matrix
	glm::mat4 translate = glm::translate(glm::mat4(), m_objPosition);

	/************************************///Rotation Matrix
	glm::vec3 rotAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(m_RotationDegrees), m_RotAxis);

	/************************************///Scale matrix
	glm::mat4 scale = glm::scale(glm::mat4(), m_objScale);

	/************************************///Texture Scale matrix
	glUniform1f(glGetUniformLocation(m_program, "fStartX"), 0.0f);
	glUniform1f(glGetUniformLocation(m_program, "fStartY"), 0.0f);

	glUniform1f(glGetUniformLocation(m_program, "fEndX"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "fEndY"), 1.0f);
	/********************************/

	glm::mat4 Model = translate * rotation * scale;
	GLint ModelLoc = glGetUniformLocation(m_program, "model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	glm::mat4 MVP;
	/********************************///MVP

	glm::mat4 view = CCameraManager::GetInstance().GetCam()->GetView();
	glm::mat4 proj = CCameraManager::GetInstance().GetCam()->GetProj();
	glUniform3fv(glGetUniformLocation(m_program, "camPos"), 1, glm::value_ptr(CCameraManager::GetInstance().GetCam()->GetCamPos()));

	MVP = proj * view;


	GLint MVPLoc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
	/***********************************/

	glUniform1f(glGetUniformLocation(m_program, "alpha"), m_Alpha);
	glUniform1i(glGetUniformLocation(m_program, "isTextured"), 1);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_program, "tex"), 0);
	glUniform3fv(glGetUniformLocation(m_program, "setColor"), 1, glm::value_ptr(m_color));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_vertices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

}