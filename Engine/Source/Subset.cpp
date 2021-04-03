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

void Subset::Render()
{
	if (nullptr == m_pVertexBuffer || nullptr == m_pIndexBuffer)
		return;

	m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, m_tVertexBufferDesc.nStride);
	m_pDevice->SetVertexDeclaration(m_tVertexBufferDesc.pVertexDecl);
	m_pDevice->SetIndices(m_pIndexBuffer);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_tVertexBufferDesc.nNumVertices, 0, m_tVertexBufferDesc.nNumFaces);
}

const LPDIRECT3DVERTEXBUFFER9 Subset::GetVertexBuffer()
{
	return m_pVertexBuffer;
}

const LPDIRECT3DINDEXBUFFER9 Subset::GetIndexBuffer()
{
	return m_pIndexBuffer;
}

const VERTEXBUFFERDESC& Subset::GetVertexBufferDesc()
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_tVertexBufferDesc;
}

const MATERIAL& Subset::GetMaterial()
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_tMaterial;
}

void Subset::Initialize(LPDIRECT3DVERTEXBUFFER9 const _pVB, LPDIRECT3DINDEXBUFFER9 const _pIB, const VERTEXBUFFERDESC& _tVBDesc, const MATERIAL& _tMaterial)
{
	m_pVertexBuffer		= _pVB;
	m_pIndexBuffer		= _pIB;
	m_tVertexBufferDesc = _tVBDesc;
	m_tMaterial			= _tMaterial;
}

