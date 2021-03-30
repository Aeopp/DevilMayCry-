#include "Subset.h"

USING(ENGINE)

Subset::Subset(LPDIRECT3DDEVICE9 const _pDevice)
	: _Device(_pDevice)
	, VB(nullptr)
	, IB(nullptr)
{
	SafeAddRef(_pDevice);
}

void Subset::Free()
{
	SafeRelease(VB);
	SafeRelease(IB);
	SafeRelease(_VtxDecl);
	SafeRelease(_Device);
	
	Object::Free();
}

Subset* Subset::Create(LPDIRECT3DDEVICE9 const _pDevice)
{
	Subset* _Ptr = new Subset(_pDevice);
	return _Ptr;
}

void Subset::Initialize(
	LPDIRECT3DVERTEXBUFFER9      const _pVB, 
	LPDIRECT3DINDEXBUFFER9       const _pIB, 
	LPDIRECT3DVERTEXDECLARATION9 const _pVtxDecl,
	const uint32 _VertexSize,
	const uint32  _NumFaces,
	const uint32  _NumVertices ,
	const uint32 _VtxBufSize,
	const Material& _Material)
{
	VertexSize   = _VertexSize;
	NumFaces     = _NumFaces;
	NumVertices  = _NumVertices;
	VtxBufSize  = 	_VtxBufSize;
	this->_Material = _Material;
	VB       = _pVB;
	IB       = _pIB;
	_VtxDecl =_pVtxDecl;
}
