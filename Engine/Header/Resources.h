#ifndef __RESOURCES_H__
#define __RESOURCES_H__
#include "EngineStdafx.h"
BEGIN(ENGINE)
class ResourceSystem;
class ENGINE_DLL Resources abstract
{
private:
	static std::weak_ptr<ResourceSystem> m_pResourceSystem;
public:
	template <typename TYPE>
	static std::shared_ptr<TYPE> Load(const std::filesystem::path _Path);
};
END
#include "Resources.hpp"
#endif // !__RESOURCES_H__