#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "StaticMesh.h"

USING(ENGINE)

StaticMesh::StaticMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Mesh(_pDevice)
{
}

StaticMesh::StaticMesh(const StaticMesh& _rOther)
	: Mesh(_rOther.m_pDevice)
{
}

void StaticMesh::Free()
{
	Mesh::Free();
}

StaticMesh* StaticMesh::Create(LPDIRECT3DDEVICE9 const _pDevice, const std::filesystem::path _Path)
{
	StaticMesh* pInstance = new StaticMesh(_pDevice);
	if (FAILED(pInstance->LoadMeshFromFile(_Path)))
	{
		pInstance->Free();
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

Resource* StaticMesh::Clone()
{
	StaticMesh* pClone = new StaticMesh(*this);
	return pClone;
}

HRESULT StaticMesh::LoadMeshFromFile(const std::filesystem::path _sPath)
{
	return S_OK;
}
