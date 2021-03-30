#include "Subset.h"

USING(ENGINE)

Subset::Subset(LPDIRECT3DDEVICE9 const _pDevice)
	: m_pDevice(_pDevice)
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
