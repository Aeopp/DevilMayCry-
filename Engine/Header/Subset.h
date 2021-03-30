#ifndef __SUBSET_H__
#define __SUBSET_H__
#include "Object.h"
BEGIN(ENGINE)
class Subset final : public Object
{
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	DWORD	m_dwStride;
	UINT	m_nNumFaces;
	UINT	m_nNumVertices;
	UINT	m_nNumPrimitive;
private:
	explicit Subset(LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~Subset() = default;
	// Object을(를) 통해 상속됨
	virtual void Free() override;
public:
	static Subset* Create(LPDIRECT3DDEVICE9 const _pDevice);
public:
	void Initialize(
		LPDIRECT3DVERTEXBUFFER9 const _pVB,
		LPDIRECT3DINDEXBUFFER9 const _pIB,
		DWORD _dwStride,
		UINT	_nNumFaces,
		UINT	_nNumVertices,
		UINT	_nNumPrimitive);
};
END
#endif // !__SUBSET_H__