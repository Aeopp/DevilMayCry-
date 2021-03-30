#include "SubMesh.h"

USING(ENGINE)

SubMesh::SubMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: m_pDevice(_pDevice)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwStride(0)
	, m_nNumFaces(0)
	, m_nNumVertices(0)
	, m_nNumPrimitive(0)
{
	SafeAddRef(m_pDevice);
}

void SubMesh::Free()
{
	SafeRelease(m_pVB);
	SafeRelease(m_pIB);
	SafeRelease(m_pDevice);
	Object::Free();
}
