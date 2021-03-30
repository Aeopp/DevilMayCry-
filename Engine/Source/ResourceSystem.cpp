#include "ResourceSystem.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(ResourceSystem)

ResourceSystem::ResourceSystem()
	: m_pDevice(nullptr)
{
}

void ResourceSystem::Free()
{
	SafeRelease(m_pDevice);
	Object::Free();
}

HRESULT ResourceSystem::ReadyResourceSystem(LPDIRECT3DDEVICE9 const _pDevice)
{
	m_pDevice = _pDevice;
	SafeAddRef(m_pDevice);
	return S_OK;
}
