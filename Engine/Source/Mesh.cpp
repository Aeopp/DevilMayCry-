#include "Mesh.h"

USING(ENGINE)

Mesh::Mesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
{
}

Mesh::Mesh(const Mesh& _rOther)
	: Resource(_rOther.m_pDevice)

{
}

void Mesh::Free()
{
	Resource::Free();
}