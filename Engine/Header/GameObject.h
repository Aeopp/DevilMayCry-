#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include "Object.h"
BEGIN(ENGINE)
class Component;
class ENGINE_DLL GameObject abstract : public Object
{
private:
	typedef std::unordered_map<size_t, std::shared_ptr<Component>> COMPONENTS;
private:
	COMPONENTS	m_Components;
	bool		m_bActive;
protected:
	explicit GameObject();
	virtual ~GameObject() = default;
	// Object을(를) 통해 상속됨
	virtual void Free() PURE;
public:
	virtual HRESULT Ready()								PURE;
	virtual HRESULT Awake()								PURE;
	virtual HRESULT Start()								PURE;
	virtual UINT	Update(const float _fDeltaTime)		PURE;
	virtual	UINT	LateUpdate(const float _fDeltaTime)	PURE;
	virtual HRESULT	Render()							PURE;

public:
	template <typename TYPE>
	std::weak_ptr<TYPE>	AddComponent();
	template <typename TYPE>
	std::weak_ptr<TYPE>	GetComponent();
public:
	bool	IsActive();
	void	SetActive(const bool _bActive);
};
END
#include "GameObject.hpp"
#endif // !__GAMEOBJECT_H__

