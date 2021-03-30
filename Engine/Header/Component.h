#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "Object.h"
BEGIN(ENGINE)
class GameObject;
class Component abstract : public Object
{
protected:
	std::weak_ptr<GameObject>	m_pGameObject;
	bool						m_bActive;
protected:
	explicit Component(std::weak_ptr<GameObject> const _pGameObject);
	virtual ~Component() = default;
	// Object��(��) ���� ��ӵ�
	virtual void Free() PURE;
public:
	bool		IsActive();
	void		SetActive(const bool _bActive);
};
END
#endif // !__COMPONENT_H__