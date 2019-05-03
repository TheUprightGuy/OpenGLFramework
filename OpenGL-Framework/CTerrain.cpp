#include "CTerrain.h"

#include <fstream>
#include <sstream>

CTerrain::CTerrain()
{
	m_Info.HeightScale = 0.35f;
	m_Info.HeightOffset = -20.0f;
	m_Info.NumRows = 513;
	m_Info.NumCols = 513;
	m_Info.CellSpacing = 1.0f;
}

CTerrain::CTerrain(TerrainInfo& initInfo)
 :m_Info(initInfo)
{
	m_program = (CProgrammerManager::GetInstance().GetProgram(DEFAULT));
	m_color = glm::vec3(0.0f, 0.0f, 0.0f);
	m_objPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_objScale = glm::vec3(1.0f, 1.0f, 1.0f);

	m_RotAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_RotationDegrees = 0.0f;

	m_Alpha = 1.0f;
}


CTerrain::~CTerrain()
{
}


// Get the heightmap from the RAW file
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
		m_Heightmap[i] = (float)in[i] * m_Info.HeightScale + m_Info.HeightOffset;
	}

}


// Compute the average height of IJ
float CTerrain::average(int i, int j)
{
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	for (UINT m = i - 1; m <= i + 1; ++m)
	{
		for (UINT n = j - 1; n <= j + 1; ++n)
		{
			if (inBounds(m, n))
			{
				avg += m_Heightmap[m*m_Info.NumCols + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}


void CTerrain::smooth()
{
	std::vector<float> dest(m_Heightmap.size());

	for (UINT i = 0; i < m_Info.NumRows; ++i)
	{
		for (UINT j = 0; j < m_Info.NumCols; ++j)
		{
			dest[i*m_Info.NumCols + j] = average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_Heightmap = dest;
}

bool CTerrain::inBounds(UINT i, UINT j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < m_Info.NumRows &&
		j >= 0 && j < m_Info.NumCols;
}


void CTerrain::GenVB()
{
	std::vector<TerrainVertex> vertices(m_Info.NumRows*m_Info.NumCols);

	float halfWidth = (m_Info.NumCols - 1)*m_Info.CellSpacing*0.5f;
	float halfDepth = (m_Info.NumRows - 1)*m_Info.CellSpacing*0.5f;

	float du = 1.0f / (m_Info.NumCols - 1);
	float dv = 1.0f / (m_Info.NumRows - 1);

	for (UINT i = 0; i < m_Info.NumRows; ++i)
	{
		float z = halfDepth - i * m_Info.CellSpacing;
		for (UINT j = 0; j < m_Info.NumCols; ++j)
		{
			float x = -halfWidth + j * m_Info.CellSpacing;

			float y = m_Heightmap[i*m_Info.NumCols + j];
			vertices[i*m_Info.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i*m_Info.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			vertices[i*m_Info.NumCols + j].texC.x = j * du;
			vertices[i*m_Info.NumCols + j].texC.y = i * dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f*m_Info.CellSpacing);
	float invTwoDZ = 1.0f / (2.0f*m_Info.CellSpacing);
	for (UINT i = 2; i < m_Info.NumRows - 1; ++i)
	{
		for (UINT j = 2; j < m_Info.NumCols - 1; ++j)
		{
			float t = m_Heightmap[(i - 1)*m_Info.NumCols + j];
			float b = m_Heightmap[(i + 1)*m_Info.NumCols + j];
			float l = m_Heightmap[i*m_Info.NumCols + j - 1];
			float r = m_Heightmap[i*m_Info.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b)*invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l)*invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			//D3DXVec3Cross(&N, &tanZ, &tanX);
			glm::normalize(N);

			vertices[i*m_Info.NumCols + j].normal = N;
		}
	}

	m_vertices = vertices;
}


void CTerrain::GenIB()
{
	std::vector<GLuint> indices(((m_Info.NumRows - 1)*(m_Info.NumCols - 1) * 2) * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	int k = 0;
	for (UINT i = 0; i < m_Info.NumRows - 1; ++i)
	{
		for (UINT j = 0; j < m_Info.NumCols - 1; ++j)
		{
			indices[k] = i * m_Info.NumCols + j;
			indices[k + 1] = i * m_Info.NumCols + j + 1;
			indices[k + 2] = (i + 1)*m_Info.NumCols + j;

			indices[k + 3] = (i + 1)*m_Info.NumCols + j;
			indices[k + 4] = i * m_Info.NumCols + j + 1;
			indices[k + 5] = (i + 1)*m_Info.NumCols + j + 1;

			k += 6; // next quad
		}
	}

	m_indices = indices;
}


void CTerrain::initTerrain()
{
	loadHeightMap();
	smooth();

	GenVB();
	GenIB();

	GLuint VBO, EBO;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(TerrainVertex) * m_Info.NumRows*m_Info.NumCols), m_vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	IndiceCount = m_indices.size();

	m_isTextured = true;
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	int width, height;
	unsigned char* image = SOIL_load_image("ground.jpg", &width, &height, 0, SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

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

	MVP = proj * view * Model;


	GLint MVPLoc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
	/***********************************/

	glUniform1f(glGetUniformLocation(m_program, "alpha"), m_Alpha);
	glUniform1i(glGetUniformLocation(m_program, "isTextured"), m_isTextured);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_program, "tex"), 0);
	glUniform3fv(glGetUniformLocation(m_program, "setColor"), 1, glm::value_ptr(m_color));


	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, IndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
