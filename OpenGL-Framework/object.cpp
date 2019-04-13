#include "object.h"
#include <iostream>

CObject::CObject(ShaderSet _programID, MeshType _meshtype, initinfo _initinfo)
	: m_programID(_programID),
	m_program(CProgrammerManager::GetInstance().GetProgram(m_programID)),
	m_MeshType(_meshtype)
{
	m_objPosition = _initinfo.objPosition;
	m_objScale = _initinfo.objScale;
	m_texStartScale = _initinfo.texStartScale;
	m_texEndScale = _initinfo.texEndScale;
	m_RotAxis = _initinfo.RotAxis;
	m_RotationDegrees = _initinfo.RotationDegrees;
	m_color = _initinfo.color;
	m_Alpha = _initinfo.alpha;
	/**************************************************/
	if (_initinfo.imgFilepath.empty())
	{
		m_isTextured = false;
	}
	else
	{
		m_isTextured = true;
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		int width, height;
		unsigned char* image = SOIL_load_image(_initinfo.imgFilepath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	/*************************************************/
}

CObject::CObject()
{
}

CObject::~CObject()
{
}

void CObject::Render(CCamera* _pCamera)
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
	glUniform1f(glGetUniformLocation(m_program, "fStartX"), m_texStartScale.x);
	glUniform1f(glGetUniformLocation(m_program, "fStartY"), m_texStartScale.y);

	glUniform1f(glGetUniformLocation(m_program, "fEndX"), m_texEndScale.x);
	glUniform1f(glGetUniformLocation(m_program, "fEndY"), m_texEndScale.y);
	/********************************/

	glm::mat4 Model = translate * rotation * scale;
	GLint ModelLoc = glGetUniformLocation(m_program, "model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(Model));

	glm::mat4 MVP;
	/********************************///MVP
	if (_pCamera != nullptr && m_MeshType != MESH_2D_SPRITE)
	{
		glm::mat4 view = _pCamera->GetView();
		glm::mat4 proj = _pCamera->GetProj();
		glUniform3fv(glGetUniformLocation(m_program, "camPos"), 1, glm::value_ptr(CCameraManager::GetInstance().GetCam()->GetCamPos()));

		MVP = proj * view * Model;
	}
	else
	{
		glUniform3fv(glGetUniformLocation(m_program, "camPos"), 1, glm::value_ptr(m_objPosition));
		MVP = Model;
	}

	GLint MVPLoc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
	/***********************************/

	if (m_programID == DEFAULT)
	{
		glUniform1f(glGetUniformLocation(m_program, "alpha"), m_Alpha);
	}
	/***********************************/

	glUniform1i(glGetUniformLocation(m_program, "isTextured"), m_isTextured);
	if (m_isTextured)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glUniform1i(glGetUniformLocation(m_program, "tex"), 0);
	}
	glUniform3fv(glGetUniformLocation(m_program, "setColor"), 1,  glm::value_ptr(m_color));
	
	if (m_programID == REFLECTIVELIGHTING)
	{
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(m_program, "skybox"), 1);
		glUniform3fv(glGetUniformLocation(m_program, "camPos"), 1, glm::value_ptr(CCameraManager::GetInstance().GetCam()->GetCamPos()));
		glBindTexture(GL_TEXTURE_CUBE_MAP, reflectCubeMap->CMtextureID);
		
	}

	CMeshManager::GetInstance().RenderMesh(m_MeshType);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void CObject::Update()
{
}

void CObject::ChangeTexture(glm::vec2 _texStartCoords, glm::vec2 _texEndCoords)
{
	m_texStartScale = _texStartCoords;
	m_texEndScale = _texEndCoords;
}

void CObject::Translate(glm::vec3 _newPos)
{
	m_objPosition = _newPos;
}

void CObject::Scale(glm::vec3 _scale)
{
	m_objScale = _scale;
}


