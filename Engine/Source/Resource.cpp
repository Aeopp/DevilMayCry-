#include "Resource.h"

USING(ENGINE)

Resource::Resource(LPDIRECT3DDEVICE9 const _pDevice)
	: m_pDevice(_pDevice)
{
	SafeAddRef(m_pDevice);
}

Resource::Resource(const Resource& _rOther)
	: m_pDevice(_rOther.m_pDevice)
{
	SafeAddRef(m_pDevice);
}

void Resource::Free()
{
	SafeRelease(m_pDevice);
	Object::Free();
}

