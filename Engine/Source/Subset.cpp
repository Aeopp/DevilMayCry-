#include "Subset.h"

USING(ENGINE)

Subset::Subset(LPDIRECT3DDEVICE9 const _pDevice)
	: m_pDevice(_pDevice)
	, m_dwStride(0)
	, m_nNumFaces(0)
	, m_nNumPrimitive(0)
	, m_nNumVertices(0)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
{
	SafeAddRef(m_pDevice);
}

void Subset::Free()
{
	SafeRelease(m_pDevice);
	Object::Free();
}

Subset* Subset::Create(LPDIRECT3DDEVICE9 const _pDevice)
{
	Subset* pInstance = new Subset(_pDevice);
	return pInstance;
}

void Subset::Initialize(LPDIRECT3DVERTEXBUFFER9 const _pVB, LPDIRECT3DINDEXBUFFER9 const _pIB, DWORD _dwStride, UINT _nNumFaces, UINT _nNumVertices, UINT _nNumPrimitive)
{
}
