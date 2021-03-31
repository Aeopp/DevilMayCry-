#include "Mesh.h"

USING(ENGINE)

Mesh::Mesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
{
}

Mesh::Mesh(const Mesh& _rOther)
	: Resource(_rOther.m_pDevice)
	, SubSets(_rOther.SubSets)
	, LocalVertexLocations(_rOther.LocalVertexLocations)
{
}

void Mesh::Free()
{
	SubSets.clear();
	SubSets.shrink_to_fit();

	LocalVertexLocations->clear();
	LocalVertexLocations->shrink_to_fit();

	Resource::Free();
}