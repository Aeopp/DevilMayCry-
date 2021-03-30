#ifndef __SUBMESH_H__
#define __SUBMESH_H__
#include "Object.h"
BEGIN(ENGINE)
class SubMesh final : public Object
{
private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	DWORD	m_dwStride;
	UINT	m_nNumFaces;
	UINT	m_nNumVertices;
	UINT	m_nNumPrimitive;

	LPDIRECT3DDEVICE9	m_pDevice;
private:
	explicit SubMesh(LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~SubMesh() = default;
	// Object을(를) 통해 상속됨
	virtual void Free() override;
};
END
#endif // !__SUBMESH_H__