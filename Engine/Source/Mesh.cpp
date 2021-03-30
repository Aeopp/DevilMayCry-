#include "Mesh.h"

USING(ENGINE)

Mesh::Mesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
	, m_pVtxDecl(nullptr)
{
}

Mesh::Mesh(const Mesh& _rOther)
	: Resource(_rOther.m_pDevice)
	, m_vecSubMesh(_rOther.m_vecSubMesh)
	, m_pVtxDecl(_rOther.m_pVtxDecl)
{
	SafeAddRef(m_pVtxDecl);
}

void Mesh::Free()
{
	m_vecSubMesh.clear();
	m_vecSubMesh.shrink_to_fit();
	
	SafeRelease(m_pVtxDecl);
	Resource::Free();
}