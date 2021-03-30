#ifndef __RESOURCE_SYSTEM_H__
#define __RESOURCE_SYSTEM_H__
#include "Object.h"
BEGIN(ENGINE)
class ResourceSystem final : public Object
{
	DECLARE_SINGLETON(ResourceSystem)
private:
	LPDIRECT3DDEVICE9	m_pDevice;
private:
	explicit ResourceSystem();
	virtual ~ResourceSystem() = default;
	// Object을(를) 통해 상속됨
	virtual void Free() override;
public:
	HRESULT ReadyResourceSystem(LPDIRECT3DDEVICE9 const _pDevice);
public:
	template <typename TYPE>
	std::shared_ptr<TYPE> Load(const std::filesystem::path _Path);
};
END
#include"ResourceSystem.hpp"
#endif // !__RESOURCE_SYSTEM_H__

