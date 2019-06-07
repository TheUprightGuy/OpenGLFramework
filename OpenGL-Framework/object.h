#pragma once
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "utils.h"
#include "camera.h"
#include "MeshManager.h"
#include "ProgramManager.h"
#include "CCubeMap.h"
#include <string>

typedef struct OBJINIT
{
	std::string imgFilepath = "";
	CCubeMap* _reflectCubemap = nullptr;
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 objPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 objScale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 RotAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	float RotationDegrees = 0.0f;

	glm::vec2 texStartScale = glm::vec2(0.0f, 0.0f);
	glm::vec2 texEndScale = glm::vec2(1.0f, 1.0f);
	float alpha = 1.0f;

}initinfo;

class CObject
{
public:

	//Program with shaders || Filepath for texture || Enum for the meshtype (MESH_2D_SPRITE,MESH_CUBE,MESH_SPHERE)
	CObject(ShaderSet _programID, MeshType _meshtype, initinfo _initinfo);
	CObject();

	~CObject();

	//Currently defunct
	void Init(GLuint &_program, std::string _imgFilepath, MeshType _meshtype);

	void Render(CCamera* _pCamera);
	void Update();

	void ChangeTexture(glm::vec2 _texStartCoords, glm::vec2 _texEndCoords);

	void Translate(glm::vec3 _newPos);
	void Scale(glm::vec3 _scale);
	void Rotation(float _fDeg, glm::vec3 _rotAxis) 
	{ 
		m_RotationDegrees = _fDeg;
		m_RotAxis = _rotAxis;
	}

	bool Contains(glm::vec3 _coords)
	{
		float top = m_objPosition.y + m_objScale.y;
		float bottom = m_objPosition.y - m_objScale.y;
		float left = m_objPosition.x - m_objScale.x;
		float right = m_objPosition.x + m_objScale.x;

		return (_coords.x > left && _coords.x < right &&
			_coords.y > bottom && _coords.y < top);
	}

	void Alpha(float _alpha) { m_Alpha = _alpha; }
	glm::vec3 GetPos() { return(m_objPosition); }
	glm::vec3 GetScale() { return(m_objScale); }

	glm::vec3 GetRotAxis() { return(m_RotAxis); }
	float GetRot() { return(m_RotationDegrees); }

	ShaderSet GetProgID() { return(m_programID); }
	MeshType GetMeshID() { return(m_MeshType); }
protected:
	ShaderSet m_programID;
	GLuint m_program;
private:
	
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

	//color
	glm::vec3 m_color;

	float m_Alpha;
	//Mesh used
	MeshType m_MeshType;
	CCubeMap* reflectCubeMap;
};

#endif /*__SPRITE_H__*/