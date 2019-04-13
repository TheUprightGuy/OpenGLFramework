#pragma once
#include "utils.h"
#include "SpriteMesh.h"
#include "CubeMesh.h"
#include "Sphere.h"

class CMeshManager
{
public:
	~CMeshManager() {}

	static CMeshManager& GetInstance()
	{
		if (s_pMeshManager == nullptr)
		{
			s_pMeshManager = new CMeshManager();
		}

		return(*s_pMeshManager);
	}

	static void DestroyInstance()
	{
		delete s_pMeshManager;
		s_pMeshManager = nullptr;
	}

	void Init(){}

	void RenderMesh(MeshType _mesh);

protected:
	static CMeshManager* s_pMeshManager;

private:
	CMeshManager();
	CMeshManager(const CMeshManager& _kr) = delete;
	CMeshManager& operator= (const CMeshManager& _kr) = delete;

	//MeshPointers
	CSpriteMesh* m_pSpriteMesh;
	CCubeMesh* m_CubeMesh;
	Sphere* m_pSphereMesh;
};
