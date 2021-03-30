#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "Object.h"
BEGIN(ENGINE)
class GameObject;
class Component abstract : public Object
{
protected:
	bool	m_bActive;
protected:
	explicit Component(GameObject* const _pGameObject);
	virtual ~Component() = default;
	// Object을(를) 통해 상속됨
	virtual void Free() PURE;
public:
	bool		IsActive();
	void		SetActive(const bool _bActive);
};
END
#endif // !__COMPONENT_H__