#pragma once
#ifndef __PROGRAMMANAGER_H__
#define __PROGRAMMANAGER_H__

#include "utils.h"
#include "ShaderLoader.h"
enum ShaderSet
{
	DEFAULT,
	PHONGLIGHTING,
	REFLECTIVELIGHTING
};

class CProgrammerManager
{
public:
	~CProgrammerManager();

	GLuint& GetProgram(ShaderSet getShader);

	static CProgrammerManager& GetInstance();

	static void DestroyInstance();

	//Singleton setup
	/*********************************************/
protected:
	static CProgrammerManager* s_pProgrammerManager;

private:
	CProgrammerManager()
	{
		defaultProg = 0;
		phongProg = 0;
		reflectProg = 0;
	}
	CProgrammerManager(const CProgrammerManager& _kr) = delete;
	CProgrammerManager& operator= (const CProgrammerManager& _kr) = delete;
	/*********************************************/


	GLuint defaultProg;
	GLuint phongProg;
	GLuint reflectProg;
};
#endif