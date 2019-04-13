#include "ProgramManager.h"

CProgrammerManager* CProgrammerManager::s_pProgrammerManager = nullptr;

CProgrammerManager::~CProgrammerManager()
{
}


GLuint & CProgrammerManager::GetProgram(ShaderSet getShader)
{
	ShaderLoader shaderLoader;

	switch (getShader)
	{
	case DEFAULT:
		if (defaultProg == 0)
		{
			defaultProg = shaderLoader.CreateProgram("Shaders/VertexShader.vs", "Shaders/FragmentShader.fs");
		}
		return(defaultProg);
		break;
	case PHONGLIGHTING:
		if (phongProg == 0)
		{
			phongProg = shaderLoader.CreateProgram("Shaders/PhongLightingVS.vs", "Shaders/PhongLightingFS.fs");
		}
		return(phongProg);
		break;
	default:
		break;
	case REFLECTIVELIGHTING:
		if (reflectProg == 0)
		{
			reflectProg = shaderLoader.CreateProgram("Shaders/ReflectVS.vs", "Shaders/ReflectFS.fs");
		}
		return (reflectProg);
		break;
	}
}

CProgrammerManager & CProgrammerManager::GetInstance()
{
	if (s_pProgrammerManager == nullptr)
	{
		s_pProgrammerManager = new CProgrammerManager();
	}

	return(*s_pProgrammerManager);
}

void CProgrammerManager::DestroyInstance()
{
	delete s_pProgrammerManager;
	s_pProgrammerManager = nullptr;
}

