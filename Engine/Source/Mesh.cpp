#include "Mesh.h"
#include "Subset.h"

USING(ENGINE)

Mesh::Mesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
{
}

Mesh::Mesh(const Mesh& _rOther)
	: Resource(_rOther.m_pDevice)
	, m_vecSubset(_rOther.m_vecSubset)
{
}

void Mesh::Free()
{
	//Mesh SubSet «ÿ¡¶
	m_vecSubset.clear();
	m_vecSubset.shrink_to_fit();
	Resource::Free();
}
