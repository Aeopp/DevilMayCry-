#ifndef __RESOURCE_SYSTEM_HPP__
#define __RESOURCE_SYSTEM_HPP__
BEGIN(ENGINE)

template<typename TYPE>
inline std::shared_ptr<TYPE> ResourceSystem::Load(const std::filesystem::path _Path)
{
	return std::shared_ptr<TYPE>();
}
END
#endif // !__RESOURCE_SYSTEM_HPP__