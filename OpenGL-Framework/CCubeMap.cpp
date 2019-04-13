#include "CCubeMap.h"

CCubeMap::CCubeMap()
{
	GLfloat verticesCM[] = {
		// Front Face
		-4.0f, 4.0f, 4.0f,
		4.0f, 4.0f, 4.0f,
		4.0f, -4.0f, 4.0f,
		-4.0f, -4.0f, 4.0f,

		// Right Face			
		4.0f, 4.0f, 4.0f,
		4.0f, 4.0f, -4.0f,
		4.0f, -4.0f, -4.0f,
		4.0f, -4.0f, 4.0f,

		// Back Face			
		4.0f, 4.0f, -4.0f,
		-4.0f, 4.0f, -4.0f,
		-4.0f, -4.0f, -4.0f,
		4.0f, -4.0f, -4.0f,

		// Left Face			
		-4.0f, 4.0f, -4.0f,
		-4.0f, 4.0f, 4.0f,
		-4.0f, -4.0f, 4.0f,
		-4.0f, -4.0f, -4.0f,

		// Top Face				
		-4.0f, 4.0f, -4.0f,
		4.0f, 4.0f, -4.0f,
		4.0f, 4.0f, 4.0f,
		-4.0f, 4.0f, 4.0f,

		// Bottom Face			
		-4.0f, -4.0f, 4.0f,
		4.0f, -4.0f, 4.0f,
		4.0f, -4.0f, -4.0f,
		-4.0f, -4.0f, -4.0f
	};
	GLuint indicesCM[] = {
		// Front Face    // Left Face
		0, 1, 2,         12, 13, 14,
		0, 2, 3,         12, 14, 15,

		// Right Face    // Top Face
		4, 5, 6,         16, 17, 18,
		4, 6, 7,         16, 18, 19,

		// Back Face     // Bottom Face
		8, 9, 10,        20, 21, 22,
		8, 10, 11,       20, 22, 23
	};

	Program = ShaderLoaderCM.CreateProgram("Shaders/CubeMapVS.vs", "Shaders/CubeMapFS.fs");

	glGenVertexArrays(1, &CMVAO);
	glBindVertexArray(CMVAO);
	glGenBuffers(1, &CMVBO);
	glGenBuffers(1, &CMEBO);
	glGenTextures(1, &CMtextureID);

	glBindBuffer(GL_ARRAY_BUFFER, CMVBO);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(verticesCM),
		verticesCM,
		GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CMEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indicesCM),
		indicesCM,
		GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_CUBE_MAP, CMtextureID);
	
	std::vector<std::string> textureFilePaths
	{ 
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
	
	int width, height;
	unsigned char* image;

	for (GLuint i = 0; i < 6; i++) {
		std::string fullPathName = "Resources/Textures/";
		fullPathName.append(textureFilePaths[i]);

		image = SOIL_load_image(fullPathName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//CameraInstance = new CCamera();
}

CCubeMap::~CCubeMap()
{

}

void CCubeMap::Render()
{
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
	currentTime = currentTime / 2000;

	GLfloat camHeight = 3.0f;
	GLfloat radius = 4.0f;
	GLfloat camX = sin(currentTime) * radius;
	GLfloat camZ = cos(currentTime) * radius;

	glUseProgram(Program);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CMtextureID);
	glUniform1i(glGetUniformLocation(Program, "cubeSampler"), 0);

	glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(1000.0f, 1000.0f, 1000.0f));

	glm::mat4 CMview = glm::lookAt(glm::vec3(camX, camHeight, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 CMproj = glm::perspective(45.0f, (GLfloat)GLUT_SCREEN_WIDTH / (GLfloat)GLUT_SCREEN_HEIGHT, 0.1f, 10000.0f);
	//glm::mat4 MVP = CMproj * CMview * model;

	glm::mat4 MVP = CCameraManager::GetInstance().GetCam()->GetProj() * CCameraManager::GetInstance().GetCam()->GetView() * model;

	glUniformMatrix4fv(glGetUniformLocation(Program, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glBindVertexArray(CMVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}