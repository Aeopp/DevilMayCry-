#include "DynamicMesh.h"

USING(ENGINE)

DynamicMesh::DynamicMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Mesh(_pDevice)
{
}

DynamicMesh::DynamicMesh(const DynamicMesh& _rOther)
	: Mesh(_rOther.m_pDevice)
{
}

void DynamicMesh::Free()
{
	Mesh::Free();
}

DynamicMesh* DynamicMesh::Create(LPDIRECT3DDEVICE9 const _pDevice, const TSTRING& _sFilePath)
{
	DynamicMesh* pInstance = new DynamicMesh(_pDevice);
	return pInstance;
}

Resource* DynamicMesh::Clone()
{
	DynamicMesh* pClone = new DynamicMesh(*this);
	return pClone;
}
