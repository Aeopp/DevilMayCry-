#ifndef __RESOURCES_HPP__
#define __RESOURCES_HPP__
BEGIN(ENGINE)
template<typename TYPE>
inline std::shared_ptr<TYPE> Resources::Load(const std::filesystem::path _Path)
{
	if (nullptr == m_pResourceSystem.get())
		return std::shared_ptr<TYPE>();
	return m_pResourceSystem.lock()->Load<TYPE>(_sPath);
}
END
#endif // !__RESOURCES_HPP__
