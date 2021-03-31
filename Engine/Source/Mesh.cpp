#include "Mesh.h"

USING(ENGINE)

Mesh::Mesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
{
}

Mesh::Mesh(const Mesh& _rOther)
	: Resource(_rOther.m_pDevice)
<<<<<<< HEAD

{
=======
	, SubSets(_rOther.SubSets)
	, LocalVertexLocations(_rOther.LocalVertexLocations)
{

>>>>>>> 00ae5e524aa64c4216e61ab5f75e320caa4e0c6d
}

void Mesh::Free()
{
<<<<<<< HEAD
=======
	SubSets->clear();
	SubSets->shrink_to_fit();

	LocalVertexLocations->clear();
	LocalVertexLocations->shrink_to_fit();

>>>>>>> 00ae5e524aa64c4216e61ab5f75e320caa4e0c6d
	Resource::Free();
}