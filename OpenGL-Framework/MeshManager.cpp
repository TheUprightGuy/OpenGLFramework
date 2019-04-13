#include "MeshManager.h"

CMeshManager* CMeshManager::s_pMeshManager = nullptr;

CMeshManager::CMeshManager()
{

}

//mesh2dsprite simply just doesn't work. if cube mesh works then the sprite should, but it just doesn't.
//Use cube scaled on the z to 0 instead
void CMeshManager::RenderMesh(MeshType _mesh)
{
	switch (_mesh)
	{
	case MESH_2D_SPRITE:
		if (m_pSpriteMesh == nullptr)
		{
			m_pSpriteMesh = new CSpriteMesh();
		}
		m_pSpriteMesh->Render();
		break;
	case MESH_CUBE:
		if (m_CubeMesh == nullptr)
		{
			m_CubeMesh = new CCubeMesh();
		}
		m_CubeMesh->Render();
		break;
	case MESH_SPHERE:
		if (m_pSphereMesh == nullptr)
		{
			m_pSphereMesh = new Sphere();
		}
		m_pSphereMesh->Render();
		break;
	default:
		break;
	}
}


