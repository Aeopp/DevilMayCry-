#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include "Object.h"
BEGIN(ENGINE)
class Scene;
class Component;
class ENGINE_DLL GameObject abstract : public Object
{
	friend Scene;
private:
	typedef std::unordered_map<size_t, std::shared_ptr<Component>> COMPONENTS;
private:
	COMPONENTS	m_Components;
	Scene*		m_pScene;
	bool		m_bActive;
	bool		m_bStatic;
	bool		m_bDestroy;
	//GameObject가 속한 Scene의 LoopPool의 어느 단계에 있는지 보관하는 변수.
	UINT		m_nLoopIdx;
protected:
	std::weak_ptr<GameObject>	m_pGameObject;
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

	virtual void	OnEnable()							PURE;
	virtual void	OnDisable()							PURE;
public:
	template <typename TYPE>
	std::weak_ptr<TYPE>	AddComponent();
	template <typename TYPE>
	std::weak_ptr<TYPE>	GetComponent();
	template <typename TYPE>
	std::weak_ptr<TYPE> AddGameObject();
public:
	void	Destroy(std::weak_ptr<GameObject> const _pGameObject);

	bool	IsActive();
	void	SetActive(const bool _bActive);
	bool	IsStatic();
	void	SetStatic(const bool _bStatic);
	
private:
	void	SetScene(Scene* const _pScene);
	void	SetGameObject(std::weak_ptr<GameObject> _pGameObject);
	UINT	GetLoopIdx();
	void	SetLoopIdx(const UINT _nLoopIdx);
};
END
#include "GameObject.hpp"
#endif // !__GAMEOBJECT_H__

