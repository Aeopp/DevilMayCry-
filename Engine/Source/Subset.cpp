#include "Subset.h"

USING(ENGINE)
Subset::Subset(LPDIRECT3DDEVICE9 const _pDevice)
	: m_pDevice(_pDevice)
	, m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
{
	SafeAddRef(_pDevice);
}

void Subset::Free()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pDevice);
	Object::Free();
}

Subset* Subset::Create(LPDIRECT3DDEVICE9 const _pDevice)
{
	Subset* pInstance = new Subset(_pDevice);
	return pInstance;
}

void Subset::Initialize(LPDIRECT3DVERTEXBUFFER9 const _pVB, LPDIRECT3DINDEXBUFFER9 const _pIB, const VERTEXBUFFERDESC& _tVBDesc, const MATERIAL& _tMaterial)
{
	m_pVertexBuffer		= _pVB;
	m_pIndexBuffer		= _pIB;
	m_tVertexBufferDesc = _tVBDesc;
	m_tMaterial			= _tMaterial;
}

