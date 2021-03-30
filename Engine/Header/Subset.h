#ifndef __SUBSET_H__
#define __SUBSET_H__
#include "Material.h"
#include "Object.h"

BEGIN(ENGINE)
class Subset final : public Object
{
public : 
	LPDIRECT3DVERTEXBUFFER9	VB { nullptr }; 
	LPDIRECT3DINDEXBUFFER9	IB{ nullptr }; 
	LPDIRECT3DVERTEXDECLARATION9 _VtxDecl{ nullptr };
	LPDIRECT3DDEVICE9	_Device{ nullptr }; 
	uint32 VtxBufSize  = 0u; 
	uint32 VertexSize  = 0u; 
	uint32 NumFaces    = 0u; 
	uint32 NumVertices = 0u; 
	Material _Material {};
private:
	explicit Subset(LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~Subset() = default;
	// Object을(를) 통해 상속됨
	virtual void Free() override;
public:
	static Subset* Create(LPDIRECT3DDEVICE9 const _pDevice);

	void Initialize(
		LPDIRECT3DVERTEXBUFFER9 const _pVB,
		LPDIRECT3DINDEXBUFFER9 const _pIB,
		LPDIRECT3DVERTEXDECLARATION9 const _pVtxDecl , 
		const uint32  _VertexSize,
		const uint32  _NumFaces,
		const uint32  _NumVertices,
		const uint32 _VtxBufSize ,
		const Material& _Material);
};
END
#endif // !__SUBSET_H__