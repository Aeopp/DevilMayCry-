#include "Mesh.h"

USING(ENGINE)

Mesh::Mesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
	, m_pVtxDecl(nullptr)
{
}

Mesh::Mesh(const Mesh& _rOther)
	: Resource(_rOther.m_pDevice)
	, m_pVtxDecl(_rOther.m_pVtxDecl)
	, m_vecSubset(_rOther.m_vecSubset)
	, m_vecMaterial(_rOther.m_vecMaterial)
{
	SafeAddRef(m_pVtxDecl);
}

void Mesh::Free()
{
	SafeRelease(m_pVtxDecl);
	Resource::Free();
}

Resource* Mesh::Clone()
{
	return nullptr;
}